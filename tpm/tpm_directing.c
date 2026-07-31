#include <pthread.h>

#include "thread_pool_manager.h"

// directing
bool tpm_stop(t_tpm *tpm) {
	if (!tpm) {
		LOG(C_MINI__TPM_LOG_EARG);
		return (false);
	}
	pthread_mutex_lock(&tpm->q_lock);
	tpm->internal_val.shutting_down = true;
	pthread_mutex_unlock(&tpm->q_lock);
	pthread_cond_signal(&tpm->q_cond);
	LOG(C_MINI__TPM_LOG_SUCCESS);
	return (true);
}
