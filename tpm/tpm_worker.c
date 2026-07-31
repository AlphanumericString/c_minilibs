#include <pthread.h>
#include <unistd.h>

#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"
pthread_mutex_t console_lock = PTHREAD_MUTEX_INITIALIZER;

void *tpmi_worker(void *t) {
	t_tpm          *tpm = (t_tpm *)t;
	struct timespec tmp;

	if (!t) return (NULL);
	if (clock_gettime(CLOCK_MONOTONIC, &tmp) != 0) return (NULL);
	tmp.tv_sec += 20;
	while (1) {
		pthread_mutex_lock(&tpm->q_lock);
		while (!tpm->internal_val.shutting_down && !tpm->q_start) {
			pthread_cond_timedwait(&tpm->q_cond, &tpm->q_lock, &tmp);
			tmp.tv_sec += 4;
		}
		if (tpm->internal_val.shutting_down) {
			pthread_mutex_unlock(&tpm->q_lock);
			break;
		}
		pthread_mutex_unlock(&tpm->q_lock);
		LOG(C_MINI__TPM_LOG_ETODO "\t worker sleep to simulate worker busy");
		sleep(1);
	}
	return (NULL);
}
