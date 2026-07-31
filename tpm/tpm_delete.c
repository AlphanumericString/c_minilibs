#include <pthread.h>
#include <stdlib.h>

#include "thread_pool_manager.h"

void tpm_delete(t_tpm *tpm) {
	void *val;

	if (!tpm) {
		LOG(C_MINI__TPM_LOG_EARG);
		return;
	}
	tpm_stop(tpm);

	size_t nb = atomic_load(&tpm->nb_threads);
	pthread_mutex_lock(&tpm->q_lock);
	pthread_cond_broadcast(&tpm->q_cond);
	pthread_mutex_unlock(&tpm->q_lock);
	for (size_t i = 0; i < nb; i++) {
		pthread_join(tpm->trd_ids[i].trd, &val);
		DBG("i:%zu, nb:%zu", i, nb);
		DBG("val:%p", val);
		(void)val;
	}
	LOG(C_MINI__TPM_LOG_SUCCESS);
	return;
}

void tpm_free(t_tpm *tpm) {
	if (!tpm) {
		LOG(C_MINI__TPM_LOG_EARG);
		return;
	}
	tpm_delete(tpm);
	free(tpm);
}
