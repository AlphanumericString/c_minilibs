#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"

/**
 * \brief allocate and initialize a new thread pool manager
 * \param nb number of worker threads to spawn, 0 uses
 * C_MINI__TPM_THREADS_DFLT_NB
 * \return pointer to the new thread pool manager, NULL on failure
 * \note nb must not exceed C_MINI__TPM_THREADS_MAX_NB
 * \ingroup tpm
**/
t_tpm *tpm_new(size_t nb) {
	t_tpm *ret;

	if (!nb) nb = C_MINI__TPM_THREADS_DFLT_NB;
	if (nb > C_MINI__TPM_THREADS_MAX_NB) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return (NULL);
	}
	if ((ret = malloc(sizeof(*ret))) == NULL) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_EALLOC)
		return (NULL);
	}
	if (tpm_init(ret, nb) == NULL) {
		free(ret);
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_ETHREADS)
		return (NULL);
	}
	return (ret);
}

// note:
// CLOCK_MONOTONIC so the timed waits (steal retry and graceful
// shutdown) are immune to wall clock changes.
static inline bool init_q(t_tpm *tpm) {
	pthread_condattr_t cattr;

	tpm->q_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	pthread_condattr_init(&cattr);
	pthread_condattr_setclock(&cattr, CLOCK_MONOTONIC);
	if (pthread_cond_init(&tpm->q_cond, &cattr)) {
		// legacy way to signal no good init, do not rely on
		atomic_store(&tpm->nb_threads, -1);
		pthread_condattr_destroy(&cattr);
		return (false);
	}
	pthread_condattr_destroy(&cattr);
	return (true);
}

static inline void init_trd(t_tpmi_thread *th, size_t id, t_tpm *const tpm) {
	th->pool           = tpm;
	th->id_sequential  = id;
	th->job_cache_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
}

/**
 * \brief initialize a caller-owned thread pool manager and spawn its workers
 * \param tpm pointer to the thread pool manager to initialize
 * \param nb_thread number of worker threads to spawn, 0 uses
 * C_MINI__TPM_THREADS_DFLT_NB
 * \return tpm on success, NULL on failure
 * \note on failure the worker threads already started are cancelled and
 * nb_threads is set to -1
 * \ingroup tpm
**/
t_tpm *tpm_init(t_tpm *tpm, size_t nb_thread) {

	if (!nb_thread) nb_thread = C_MINI__TPM_THREADS_DFLT_NB;
	if (!tpm || nb_thread > C_MINI__TPM_THREADS_MAX_NB) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return (NULL);
	}
	*tpm = (t_tpm){0};
	if (init_q(tpm) != true) {
		CM_LOG(C_MINI__LEVEL_INFO, "CATTR INIT FAIL: %s", strerror(errno))
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_CATTR);
		return (NULL);
	}
	atomic_store(&tpm->nb_threads, nb_thread);
	for (size_t i = 0; i < nb_thread; i++) {
		init_trd(&tpm->threads[i], i, tpm);
		if (pthread_create(&(tpm->threads[i].id_absolute), NULL, &tpmi_worker,
		                   &tpm->threads[i])) {
			CM_LOG(C_MINI__LEVEL_INFO, "PTHREAD_CREATE FAIL: %s",
			       strerror(errno))
			tpm_stop(tpm);
			for (size_t j = 0; j < i;
			     pthread_cancel(tpm->threads[j++].id_absolute))
				;
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_ETHREADS)
			atomic_store(&tpm->nb_threads, -1);
			return (NULL);
		}
	}
	return (tpm);
}
