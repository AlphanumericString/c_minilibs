#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>

#include "thread_pool_manager.h"

void tpm_delete(t_tpm *tpm, void (*delete_func)(void *)) {
	void                *val;
	size_t               nb;
	t_tpm_job_container *nxt;

	if (!tpm) {
		LOG(C_MINI__TPM_LOG_EARG);
		return;
	}
	tpm_stop(tpm);

	nb = atomic_load(&tpm->nb_threads);
	pthread_mutex_lock(&tpm->q_lock);
	pthread_cond_broadcast(&tpm->q_cond);
	pthread_mutex_unlock(&tpm->q_lock);
	for (size_t i = 0; i < nb; i++) {
		pthread_join(tpm->threads[i].id_absolute, &val);
		(void)val;
	}
	for (t_tpm_job_container *jc = tpm->q_start; jc; jc = nxt) {
		nxt = jc->next;
		if (delete_func) delete_func(jc->job.arg);
		free(jc);
	}

	tpm_clear_job_list(tpm->q_start, NULL);
	LOG(C_MINI__TPM_LOG_SUCCESS);
	return;
}

void tpm_free(t_tpm *tpm, void (*delete_func)(void *)) {
	if (!tpm) {
		LOG(C_MINI__TPM_LOG_EARG);
		return;
	}
	tpm_delete(tpm, delete_func);
	free(tpm);
}
