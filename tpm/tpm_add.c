#include <assert.h>
#include <common_base/log.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"

#define CACHE_LEN     C_MINI__TPM_THRD_JOBS_CACHE
#define job_container t_tpm_jc

int tpmi_job_thrd_cache_pop(t_tpm *tpm, job_container *buffer[CACHE_LEN],
                            size_t start) {
	size_t         i = start;
	job_container *it;

	if (!tpm || !buffer) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return (-1);
	}
	it = tpm->q_start;
	for (; i < CACHE_LEN && it; i++) {
		buffer[i] = it;
		it        = it->next;
	}
	if (!it) {
		tpm->q_start = NULL;
		tpm->q_end   = NULL;
	} else {
		if (it->next) it->next->prev = NULL;
		tpm->q_start = it->next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, "Success: nb jobs taken:%zu", i - start)
	return (i - start);
}

job_container *tpm_add_job(t_tpm *tpm, t_work work, void *arg) {
	job_container *j;
	bool           r;

	if (!tpm || !work) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return (NULL);
	}
	j = tpmi_create_job(work, arg);
	if (!j) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_EALLOC)
		return (NULL);
	}
	pthread_mutex_lock(&tpm->q_lock);
	r = tpmi_job_push(j, &tpm->q_start, &tpm->q_end,
	                  tpm->internal_val.queue_closed);
	pthread_mutex_unlock(&tpm->q_lock);
	if (!r) return (free(j), NULL);
	pthread_cond_signal(&tpm->q_cond);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
	return (j);
}
job_container *tpm_add_reccuring_job(t_tpm *tpm, t_work work, void *arg) {
	job_container *j;
	bool           r;

	if (!tpm || !work) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return (NULL);
	}
	j = tpmi_create_job(work, arg);
	if (!j) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_EALLOC)
		return (NULL);
	}
	j->job.infos.reccuring = true;
	pthread_mutex_lock(&tpm->q_lock);
	r = tpmi_job_push(j, &tpm->q_start, &tpm->q_end,
	                  tpm->internal_val.queue_closed);
	pthread_mutex_unlock(&tpm->q_lock);
	if (!r) return (free(j), NULL);
	pthread_cond_signal(&tpm->q_cond);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
	return (j);
}
job_container *tpm_add_batch(t_tpm *tpm, size_t nb, t_work work, void **args) {
	job_container *local_head;
	job_container *it;
	bool           shuttingdown;

	if (!tpm || !work || !args) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return (NULL);
	}
	if (!nb) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
		return (NULL);
	}
	pthread_mutex_lock(&tpm->q_lock);
	shuttingdown = tpm->internal_val.shutting_down;
	pthread_mutex_unlock(&tpm->q_lock);
	if (shuttingdown) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EDOWN)
		return (NULL);
	}
	local_head = tpmi_create_job(work, args[0]);
	it         = local_head;
	for (size_t i = 1; i < nb; i++) {
		it->next = tpmi_create_job(work, args[i]);
		if (!it->next) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_EALLOC)
			tpm_clear_job_list(local_head, NULL);
			return (NULL);
		}
		it->next->prev = it;
		it             = it->next;
	}

	pthread_mutex_lock(&tpm->q_lock);
	it->next = tpm->q_start;
	if (tpm->q_start) tpm->q_start->prev = it->next;
	if (!tpm->q_end) tpm->q_end = it;
	tpm->q_start = local_head;
	pthread_mutex_unlock(&tpm->q_lock);
	return (NULL);
}

void tpm_clear_job_list(job_container *head, t_call cleanup) {
	for (job_container *nxt; head; head = nxt) {
		nxt = head->next;
		if (cleanup) cleanup(head->job.arg);
		free(head);
	}
}
