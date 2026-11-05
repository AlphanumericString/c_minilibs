#include <pthread.h>

#include "thread_pool_manager.h"

/**
 * \brief read a snapshot of the pool statistics
 * \param tpm thread pool manager to read stats from
 * \return stats snapshot; on error both fields are set to (size_t)-1
 * \ingroup tpm
**/
struct s_tpm_stats tpm_get_stats(t_tpm *tpm) {
	struct s_tpm_stats r;

	if (!tpm) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return ((struct s_tpm_stats){.total_jobs_failed    = -1,
		                             .total_jobs_completed = -1});
	}
	pthread_mutex_lock(&tpm->q_lock);
	r = tpm->stats;
	pthread_mutex_unlock(&tpm->q_lock);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
	return (r);
}
/**
 * \brief get the number of worker threads in the pool
 * \param tpm thread pool manager to inspect
 * \return number of worker threads, 0 if tpm is NULL
 * \ingroup tpm
**/
size_t tpm_get_nbthread(const t_tpm *tpm) {
	size_t nb;

	if (!tpm) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return (0);
	}
	nb = atomic_load(&tpm->nb_threads);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
	return (nb);
}
