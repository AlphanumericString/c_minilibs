#include <pthread.h>

#include "thread_pool_manager.h"

struct s_tpm_stats tpm_get_stats(t_tpm *tpm) {
	struct s_tpm_stats r;

	if (!tpm) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG);
		return ((struct s_tpm_stats){.total_jobs_failed    = -1,
		                             .total_jobs_completed = -1});
	}
	pthread_mutex_lock(&tpm->q_lock);
	r = tpm->stats;
	pthread_mutex_unlock(&tpm->q_lock);
	CM_LOG(C_MINI__LEVEL_ALWAYS, C_MINI__TPM_LOG_ETODO);
	return (r);
}
int tpm_get_nbthread(const t_tpm *tpm) {
	int nb;

	if (!tpm) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG);
		return (0);
	}
	nb = atomic_load(&tpm->nb_threads);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS);
	return (nb);
}
