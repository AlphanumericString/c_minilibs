#include <assert.h>
#include <pthread.h>
#include <unistd.h>

#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"

pthread_mutex_t console_lock = PTHREAD_MUTEX_INITIALIZER;

void *tpmi_worker(void *t) {
	t_tpm_thread_internal th = *(t_tpm_thread_internal *)t;
	t_tpm_job_container   c;
	void                 *jr;
	t_tpm                *tpm = th.pool;

	//	t_thread_stats	*local_stats;

	if (!t) {
		LOG(C_MINI__TPM_LOG_EARG);
		return (NULL);
	}
	while (1) {
		pthread_mutex_lock(&tpm->q_lock);
		while (!tpm->internal_val.shutting_down && !tpm->q_start) {
			pthread_cond_wait(&tpm->q_cond, &tpm->q_lock);
			LOG(C_MINI__TPM_LOG_TRD_INFO, th.id_sequential, "Awake");
		}
		if (tpm->internal_val.shutting_down) {
			pthread_mutex_unlock(&tpm->q_lock);
			LOG(C_MINI__TPM_LOG_TRD_INFO, th.id_sequential, "Shutting down");
			break;
		}
		c = tpmi_job_pop(tpm);
		pthread_mutex_unlock(&tpm->q_lock);
		LOG(C_MINI__TPM_LOG_TRD_INFO "\tjob:%p", th.id_sequential,
		    "Job logging", c.job.func);

		assert(c.job.func != NULL);
		jr = c.job.func(c.job.arg);

		LOG(C_MINI__TPM_LOG_TRD_INFO "\tres:%p", th.id_sequential, "Result",
		    jr);
	}
	//	return (local_stats);
	return (NULL);
}
