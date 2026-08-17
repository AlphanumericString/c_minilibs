#include <assert.h>
#include <pthread.h>
#include <stdint.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"

pthread_mutex_t console_lock = PTHREAD_MUTEX_INITIALIZER;

// const int              victims_window = MIN(4, tpm->nb_threads - 1);
// ->
// const int              victims_window = MIN(8, log2(tpm->nb_threads - 1));
//
// batch_refill_from_mainqueue: false fail, true success
static inline bool batch_refill_from_mainqueue(t_tpm            *tpm,
                                               t_tpm_int_thread *th) {
	LOG(C_MINI__TPM_LOG_TRD_INFO, th->id_sequential,
	    "steal from main queue try");

	int nb_jbs;

	// when are we stealing?
	pthread_mutex_lock(&th->job_cache_lock);
	nb_jbs  = tpmi_job_thrd_cache_pop(tpm, th->jc, th->start);
	th->end = nb_jbs;
	pthread_mutex_unlock(&th->job_cache_lock);
	if (!nb_jbs || nb_jbs == th->start) return (false);
	return (true);
}

static inline t_tpm_int_thread *get_victim(t_tpm *tpm, t_tpm_int_thread th) {
	static int        nv             = 0;
	const int         victims_window = MIN(4, tpm->nb_threads - 1);
	uint8_t           attempts       = 1;
	t_tpm_int_thread *victim;

	victim = &tpm->threads[(th.id_sequential + nv) % tpm->nb_threads];
	while (attempts < victims_window) {
		LOG(C_MINI__TPM_LOG_TRD_INFO "%zu", th.id_sequential,
		    "trying to steal from:", victim->id_sequential);
		if (pthread_mutex_trylock(&victim->job_cache_lock)) {
			if (victim->end - victim->start <= 0)
				pthread_mutex_unlock(&victim->job_cache_lock);
			else break;
		}
		nv     = (nv + 1) % victims_window;
		victim = &tpm->threads[(th.id_sequential + nv) % tpm->nb_threads];
		attempts++;
	}
	if (attempts == victims_window) {
		LOG(C_MINI__TPM_LOG_TRD_INFO, th.id_sequential,
		    "Failed to find a thread to steal from.");
		return (NULL);
	}
	LOG(C_MINI__TPM_LOG_TRD_INFO "from %zu", th.id_sequential, "Stealing",
	    victim->id_sequential);
	DBG(C_MINI__TPM_LOG_TRD_INFO "attempts: %u", th.id_sequential, "Stealing",
	    attempts);
	return (victim);
}
// steal_work_robinhood: false fail, true success
static inline bool steal_work_robinhood(t_tpm *tpm, t_tpm_int_thread *th) {

	t_tpm_int_thread *victimus;
	size_t            to_steal;
	LOG(C_MINI__TPM_LOG_TRD_INFO, th->id_sequential,
	    "steal from neighbor queue try");

	// pthread_mutex_lock(&th.job_cache_lock);
	if ((victimus = get_victim(tpm, *th)) == NULL) { // steal failed
		pthread_mutex_unlock(&th->job_cache_lock);
		return (false);
	}
	// WARNING: get_victim locks their job cache queue

	to_steal       = (victimus->end - victimus->start) / 2;
	// we only call steal when empty.
	// otherwise do math here
	victimus->end -= to_steal;
	memcpy(&(th->jc[th->start]), &victimus->jc[victimus->end], to_steal);
	DBG("th->end: %zu (max:%d)", th->end + to_steal,
	    C_MINI__TPM_THRD_JOBS_CACHE)
	th->end += to_steal;
	pthread_mutex_unlock(&(victimus->job_cache_lock));
	pthread_mutex_unlock(&th->job_cache_lock); // steal ok
	LOG(C_MINI__TPM_LOG_TRD_INFO "Stole %zu jobs from Thread#%zu",
	    th->id_sequential, "Steal infos:", to_steal, victimus->id_sequential);
	return (true);
}
static bool get_new_jobs(t_tpm *tpm, t_tpm_int_thread *th) {
	pthread_mutex_lock(&th->job_cache_lock);
	LOG(C_MINI__TPM_LOG_TRD_INFO "{%hhu, %hhu}", th->id_sequential,
	    "decision tree {start, end}:", th->start, th->end);
	if (!th->end || th->start == th->end) {
		pthread_mutex_unlock(&th->job_cache_lock);
		return (batch_refill_from_mainqueue(tpm, th));
	}
	// pthread_mutex_unlock(&th->job_cache_lock);
	if (steal_work_robinhood(tpm, th) == false)
		return (batch_refill_from_mainqueue(tpm, th));
	return (true);
}

void *tpmi_worker(void *t) {
	t_tpm_int_thread *th = (t_tpm_int_thread *)t;
	//	t_tpm_base_job        current_employment_contract;
	bool              linkedin;
	size_t            i;
	void             *jr;
	t_tpm            *tpm = th->pool;

	if (!t) {
		LOG(C_MINI__TPM_LOG_EARG);
		return (NULL);
	}
	while (1) {
		pthread_mutex_lock(&tpm->q_lock);
		while (!tpm->internal_val.shutting_down && !tpm->q_start) {
			pthread_cond_wait(&tpm->q_cond, &tpm->q_lock);
			LOG(C_MINI__TPM_LOG_TRD_INFO, th->id_sequential, "Awake");
		}
		if (tpm->internal_val.shutting_down) {
			pthread_mutex_unlock(&tpm->q_lock);
			LOG(C_MINI__TPM_LOG_TRD_INFO, th->id_sequential, "Shutting down");
			break;
		}
		linkedin = get_new_jobs(tpm, th); // get new jobs (steal or queue)
		pthread_mutex_unlock(&tpm->q_lock);

		if (!linkedin) { // failed to get a job .. so do you have a local job?
			pthread_mutex_lock(&th->job_cache_lock);
			linkedin =
			    !(th->start == th->end || !th->end); // local cache filled?
			pthread_mutex_unlock(&th->job_cache_lock);
		}
		if (!linkedin) continue; // no job, go get a job
		while (1) {              // exe local jobs
			pthread_mutex_lock(&th->job_cache_lock);
			DBG(C_MINI__TPM_LOG_TRD_INFO "{%hhu, %hhu}", th->id_sequential,
			    "{start, end}", th->start, th->end);
			if (th->start >= th->end) {
				break;
			}
			//	current_employment_contract = th->jc[th->start]->job;
			i = th->start;
			th->start++;
			pthread_mutex_unlock(&th->job_cache_lock);
			jr = th->jc[i]->job.func(th->jc[i]->job.arg);
			LOG(C_MINI__TPM_LOG_TRD_INFO "{%p, %p}", th->id_sequential,
			    "Job logging{job, return}\t", th->jc[i]->job.func, jr);
			(void)jr;
		}
		th->start = 0;
		th->end   = 0;
		pthread_mutex_unlock(&th->job_cache_lock);
	}
	//	return (local_stats);
	return (NULL);
}
