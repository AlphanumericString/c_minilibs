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
		LOG(C_MINI__TPM_LOG_EARG);
		return (NULL);
	}
	if ((ret = malloc(sizeof(*ret))) == NULL) {
		LOG(C_MINI__TPM_LOG_EALLOC);
		return (NULL);
	}
	tpm_init(ret, nb);
	if (atomic_load(&ret->nb_threads) == -1) {
		free(ret);
		LOG("Thread Initialization Failed.");
		return (NULL);
	}
	return (ret);
}

t_tpm *tpm_init(t_tpm *tpm, size_t nb_thread) {
	if (!nb_thread) nb_thread = TPM_THREADS_DFLT_NB;
	if (!tpm || nb_thread > TPM_THREADS_MAX_NB) {
		LOG(C_MINI__TPM_LOG_EARG);
		return (NULL);
	}

	*tpm = (t_tpm){0};
	atomic_store(&tpm->nb_threads, nb_thread);
	tpm->q_lock = (typeof(tpm->q_lock))PTHREAD_MUTEX_INITIALIZER;
	tpm->q_cond = (typeof(tpm->q_cond))PTHREAD_COND_INITIALIZER;

	for (size_t i = 0; i < nb_thread; i++) {
		tpm->threads[i].pool          = tpm;
		tpm->threads[i].id_sequential = i;
		if (pthread_create(&(tpm->threads[i].id_absolute), NULL, &tpmi_worker,
		                   &tpm->threads[i])) {
			LOG("PTHREAD_CREATE FAIL: %s", strerror(errno));
			tpm_stop(tpm);
			for (size_t j = 0; j < i;
			     pthread_cancel(tpm->threads[j++].id_absolute))
				;
			LOG(C_MINI__TPM_LOG_ETHREADS);
			atomic_store(&tpm->nb_threads, -1);
			return (tpm);
		}
	}
	return (tpm);
}
