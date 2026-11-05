#include <pthread.h>
#include <time.h>
#include <unistd.h>

#include "thread_pool_manager.h"

// directing
/**
 * \brief request a graceful shutdown of the pool
 * \param tpm thread pool manager to stop
 * \return true on success, false if tpm is NULL
 * \note closes the job queue and wakes every worker so they exit once their
 * current work is done. Jobs still in the main queue are left for tpm_delete
 * \ingroup tpm
**/
bool tpm_stop(t_tpm *tpm) {
	if (!tpm) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return (false);
	}
	pthread_mutex_lock(&tpm->q_lock);
	tpm->internal_val.shutting_down = true;
	tpm->internal_val.queue_closed  = true;
	pthread_mutex_unlock(&tpm->q_lock);
	pthread_cond_broadcast(&tpm->q_cond);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
	return (true);
}

static void wait_main_queue_empty(t_tpm *tpm, struct timespec *t,
                                  size_t *step) {
	struct timespec c;

	pthread_mutex_lock(&tpm->q_lock);
	while (tpm->q_start) {
		pthread_cond_timedwait(&tpm->q_cond, &tpm->q_lock, t);
		if (!clock_gettime(CLOCK_MONOTONIC, &c) && c.tv_sec >= t->tv_sec) {
			t->tv_sec += *step;
			*step     *= 2;
		}
	}
	pthread_mutex_unlock(&tpm->q_lock);
}

static void wait_thread_queues_empty(t_tpm *tpm) {
	bool break_order;

	for (size_t i = 0; i < tpm->nb_threads; i++) {
		break_order = false;
#if defined(C_MINI__TESTS) ||                                                  \
    defined(C_MINI__DEBUG_LEVEL) && C_MINI__DEBUG_LEVEL != 0

		pthread_mutex_lock(&tpm->threads[i].job_cache_lock);
		CM_LOG(C_MINI__LEVEL_INFO,
		       "Waiting thread#%zu {start,end}: {%hhu, %hhu}", i,
		       tpm->threads[i].start, tpm->threads[i].end)
		pthread_mutex_unlock(&tpm->threads[i].job_cache_lock);
#endif

		while (!break_order) {
			pthread_mutex_lock(&tpm->threads[i].job_cache_lock);
			if (tpm->threads[i].end == tpm->threads[i].start ||
			    !tpm->threads[i].end) {
				break_order = true;
			}
			pthread_mutex_unlock(&tpm->threads[i].job_cache_lock);
			usleep(2000);
		}
		CM_LOG(C_MINI__LEVEL_INFO, "Waiting thread#%zu - done", i)
	}
}

/**
 * \brief close the job queue and wait until all queued jobs have run
 * \param tpm thread pool manager to wait for
 * \return true on success, false if tpm is NULL or the monotonic clock failed
 * \note call this before tpm_delete to drain the pool
 * \ingroup tpm
**/
bool tpm_wait_end(t_tpm *tpm) {
	struct timespec t;
	size_t          step = 1;

	if (!tpm) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return (false);
	}
	if (clock_gettime(CLOCK_MONOTONIC, &t)) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_ECLOCK)
		return (false);
	}
	pthread_mutex_lock(&tpm->q_lock);
	tpm->internal_val.queue_closed = true;
	pthread_mutex_unlock(&tpm->q_lock);
	pthread_cond_signal(&tpm->q_cond);

	wait_main_queue_empty(tpm, &t, &step);
	wait_thread_queues_empty(tpm);

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
	return (true);
}
