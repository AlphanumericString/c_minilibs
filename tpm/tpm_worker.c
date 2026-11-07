#include <stdlib.h>
#include <string.h>
#include <sys/param.h>

#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"

pthread_mutex_t console_lock = PTHREAD_MUTEX_INITIALIZER;

// batch_refill_from_mainqueue: move up to `max` jobs from the head of the main
// queue into the worker's local cache (appended at `th->end`).
// Caller holds `th->job_cache_lock` and `tpm->q_lock`.
// false: nothing taken, true: at least one job taken.
static inline bool
batch_refill_from_mainqueue(t_cm_tpm *tpm, t_cm_tpmi_thread *th, size_t max) {
	int nb_jbs;

	nb_jbs = tpmi_job_thrd_cache_pop(tpm, th->jc, th->end, max);
	if (nb_jbs <= 0) return (false);
	th->end += nb_jbs;
	CM_LOG(C_MINI__LEVEL_INFO,
	       C_MINI__TPM_LOG_TRD_INFO "took %d from main queue",
	       th->id_sequential, "Refill:", nb_jbs)
	return (true);
}

// steal_work: inspect up to C_MINI__TPM_STEAL_WINDOW neighbours (starting after
// `th`). If any thread inspected have more than 1 job and more job that current
// thread searching for work then steal half their work onto itself.
// note: using trylock to (1) reduce contention (2) skip lock currently busy
// with filling up their own works (3) avoid dead-lock problems
// note: th's own jc lock must be set before calling this.
static inline bool steal_work(t_cm_tpm *tpm, t_cm_tpmi_thread *th) {
	size_t            nb         = atomic_load(&tpm->nb_threads);
	size_t            free_space = C_MINI__TPM_TRD_JOB_CACHE_LEN - th->end;
	size_t            mine       = th->end - th->start;
	const size_t      window     = MIN(C_MINI__TPM_STEAL_WINDOW, (nb - 1));
	bool              stole      = false;
	t_cm_tpmi_thread *th_stolen;
	size_t            other_count;
	size_t            nb_to_steal;

	if (nb <= 1) return (false);
	for (size_t a = 0; a < window && free_space; a++) {
		th_stolen = &tpm->threads[(th->id_sequential + 1 + a) % (size_t)nb];
		if (pthread_mutex_trylock(&th_stolen->job_cache_lock)) continue;
		other_count = th_stolen->end - th_stolen->start;
		if (other_count > 1 && other_count > mine) {
			nb_to_steal = other_count / 2;
			if (nb_to_steal > free_space) nb_to_steal = free_space;
			th_stolen->end -= nb_to_steal;
			memcpy(&th->jc[th->end], &th_stolen->jc[th_stolen->end],
			       nb_to_steal * sizeof(t_cm_tpm_jc *));
			th->end    += nb_to_steal;
			free_space -= nb_to_steal;
			mine       += nb_to_steal;
			stole       = true;
			CM_LOG(C_MINI__LEVEL_INFO,
			       C_MINI__TPM_LOG_TRD_INFO "Stole %zu jobs from Thread#%zu",
			       th->id_sequential, "Steal infos:", nb_to_steal,
			       th_stolen->id_sequential)
		}
		pthread_mutex_unlock(&th_stolen->job_cache_lock);
	}
	if (!stole)
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_TRD_INFO, th->id_sequential,
		       "No neighbour to steal from.")
	return (stole);
}

// get_new_jobs: take at most C_MINI__TPM_TRD_MGET_NB jobs from the main queue
// (keeps a slice of the cache free for stolen work), then try to steal from
// neighbours. If nothing could be stolen the free space is useless, so top it
// up from the main queue. Caller holds `tpm->q_lock`.
// note: when nothing was stolen the slice reserved for stealing is useless, so
// top up from the main queue. `end` is always < cache_len here: the first
// refill can only add MGET_NB (< cache_len) and a failed steal never grows
// the cache.
static bool get_new_jobs(t_cm_tpm *tpm, t_cm_tpmi_thread *th) {
	const size_t cache_len = C_MINI__TPM_TRD_JOB_CACHE_LEN;
	bool         stole;

	pthread_mutex_lock(&th->job_cache_lock);
	if (th->end > th->start) { // already holds work
		pthread_mutex_unlock(&th->job_cache_lock);
		return (true);
	}
	th->start = 0;
	th->end   = 0;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_TRD_INFO, th->id_sequential,
	       "Looking for work")
	batch_refill_from_mainqueue(tpm, th, C_MINI__TPM_TRD_MGET_NB);
	stole = steal_work(tpm, th);
	if (!stole) batch_refill_from_mainqueue(tpm, th, cache_len - th->end);
	stole = th->end > th->start;
	pthread_mutex_unlock(&th->job_cache_lock);
	return (stole);
}

// checks if work disponible if not try to wait STEAL_RETRY_NS and then check
// for work again, if clock fails, just wait for the main q_cond to signal when
// an event happened
static inline void wait_job_opportunity(t_cm_tpm *tpm, size_t thread_id) {
	struct timespec wake;

	if (!tpm->internal_val.shutting_down && !tpm->q_start) {
		if (!clock_gettime(CLOCK_MONOTONIC, &wake)) {
			wake.tv_nsec += C_MINI__TPM_STEAL_RETRY_NS;
			if (wake.tv_nsec >= 1000000000L) {
				wake.tv_sec  += 1;
				wake.tv_nsec -= 1000000000L;
			}
			pthread_cond_timedwait(&tpm->q_cond, &tpm->q_lock, &wake);
		} else {
			pthread_cond_wait(&tpm->q_cond, &tpm->q_lock);
		}
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_TRD_INFO, thread_id, "Awake")
	}
	(void)thread_id;
}

// split into function for easier loggin / job related stuff to be put here
// (such as future and give return back to user later)
static inline void *job_launcher_hook(t_cm_tpm_jc *jc, size_t thread_id) {
	t_cm_tpm_jc *j  = jc;
	t_cm_work    fn = j->job.func;
	void        *jr;

	jr = fn(j->job.arg);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_TRD_INFO "{job=%p, return=%p}",
	       thread_id, "Job logging:", fn, jr)
	(void)thread_id;
	free(j);
	return (jr);
}

void *tpmi_worker(void *t) {
	t_cm_tpmi_thread *tpi = (t_cm_tpmi_thread *)t;
	bool              linkedin;
	size_t            i;
	void             *jr;
	t_cm_tpm         *tpm;

	if (!t) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_EARG)
		return (NULL);
	}
	tpm = tpi->pool;
	while (1) {
		pthread_mutex_lock(&tpm->q_lock);
		wait_job_opportunity(tpm, tpi->id_sequential);
		if (tpm->internal_val.shutting_down) {
			pthread_mutex_unlock(&tpm->q_lock);
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_TRD_INFO,
			       tpi->id_sequential, "Shutting down")
			break;
		}
		linkedin = get_new_jobs(tpm, tpi);
		pthread_mutex_unlock(&tpm->q_lock);
		if (!linkedin) continue; // no job gotten, go get a job

		pthread_mutex_lock(&tpi->job_cache_lock);
		while (tpi->start < tpi->end) { // execute local jobs
			i = tpi->start;
			tpi->start++;
			pthread_mutex_unlock(&tpi->job_cache_lock);
			jr = job_launcher_hook(tpi->jc[i], tpi->id_sequential);
			(void)jr;
			pthread_mutex_lock(&tpi->job_cache_lock);
		}
		tpi->start = 0;
		tpi->end   = 0;
		pthread_mutex_unlock(&tpi->job_cache_lock);
	}
	return (NULL);
}
