#include <errno.h>
#include <pthread.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <string.h>

#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"

t_tpm *tpm_new(size_t nb) {
	t_tpm *ret;

	if (!nb) nb = TPM_THREADS_DFLT_NB;
	if (nb > TPM_THREADS_MAX_NB) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG);
		return (NULL);
	}
	if ((ret = malloc(sizeof(*ret))) == NULL) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_EALLOC);
		return (NULL);
	}
	tpm_init(ret, nb);
	if (atomic_load(&ret->nb_threads) == -1) {
		free(ret);
		CM_LOG(C_MINI__LEVEL_ERR, "Thread Initialization Failed.");
		return (NULL);
	}
	return (ret);
}

t_tpm *tpm_init(t_tpm *tpm, size_t nb_thread) {
	if (!nb_thread) nb_thread = TPM_THREADS_DFLT_NB;
	if (!tpm || nb_thread > TPM_THREADS_MAX_NB) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG);
		return (NULL);
	}

	*tpm = (t_tpm){0};
	atomic_store(&tpm->nb_threads, nb_thread);
	tpm->q_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	tpm->q_cond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;

	for (size_t i = 0; i < nb_thread; i++) {
		tpm->threads[i].pool          = tpm;
		tpm->threads[i].id_sequential = i;
		tpm->threads[i].job_cache_lock =
		    (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
		if (pthread_create(&(tpm->threads[i].id_absolute), NULL, &tpmi_worker,
		                   &tpm->threads[i])) {
			CM_LOG(C_MINI__LEVEL_INFO, "PTHREAD_CREATE FAIL: %s",
			       strerror(errno));
			tpm_stop(tpm);
			for (size_t j = 0; j < i;
			     pthread_cancel(tpm->threads[j++].id_absolute))
				;
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_ETHREADS);
			atomic_store(&tpm->nb_threads, -1);
			return (tpm);
		}
	}
	return (tpm);
}
