#include <pthread.h>
#include <time.h>

#include "thread_pool_manager.h"

// directing
bool tpm_stop(t_tpm *tpm) {
	if (!tpm) {
		LOG(C_MINI__TPM_LOG_EARG);
		return (false);
	}
	pthread_mutex_lock(&tpm->q_lock);
	tpm->internal_val.shutting_down = true;
	tpm->internal_val.queue_closed  = true;
	pthread_mutex_unlock(&tpm->q_lock);
	pthread_cond_broadcast(&tpm->q_cond);
	LOG(C_MINI__TPM_LOG_SUCCESS);
	return (true);
}

bool tpm_wait_end(t_tpm *tpm) {
	struct timespec t;
	struct timespec c;
	size_t          step = 1;

	if (!tpm) {
		LOG(C_MINI__TPM_LOG_EARG);
		return (false);
	}
	if (clock_gettime(CLOCK_MONOTONIC, &t)) {
		LOG(C_MINI__TPM_LOG_ECLOCK);
		return (false);
	}
	pthread_mutex_lock(&tpm->q_lock);
	tpm->internal_val.queue_closed = true;
	pthread_mutex_unlock(&tpm->q_lock);
	pthread_cond_signal(&tpm->q_cond);

	pthread_mutex_lock(&tpm->q_lock);
	while (tpm->q_start) {
		pthread_cond_timedwait(&tpm->q_cond, &tpm->q_lock, &t);
		if (!clock_gettime(CLOCK_MONOTONIC, &c) && c.tv_sec >= t.tv_sec) {
			t.tv_sec += step;
			step     *= 2;
		}
	}
	pthread_mutex_unlock(&tpm->q_lock);

	LOG(C_MINI__TPM_LOG_SUCCESS);
	return (true);
}
