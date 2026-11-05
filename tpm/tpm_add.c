#include <stdlib.h>

#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"

#define CACHE_LEN C_MINI__TPM_TRD_JOB_CACHE_LEN

int tpmi_job_thrd_cache_pop(t_cm_tpm *tpm, t_cm_tpm_jc **buffer, size_t start,
                            size_t max) {
	size_t       i = 0;
	t_cm_tpm_jc *it;

	if (!tpm || !buffer) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return (-1);
	}
	it = tpm->q_start;
	for (; i < max && it; i++) {
		buffer[start + i] = it;
		it                = it->next;
	}
	if (!it) {
		tpm->q_start = NULL;
		tpm->q_end   = NULL;
	} else {
		it->prev     = NULL;
		tpm->q_start = it;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS "Jobs taken:%zu", i)
	return ((int)i);
}

/**
 * \brief enqueue a single job in the thread pool manager
 * \param tpm thread pool manager to enqueue the job in
 * \param work function to execute as the job
 * \param arg argument passed to work when the job runs
 * \return handle to the job container, NULL on failure
 * \note the returned handle is only valid until the job completes
 * \ingroup tpm
**/
t_cm_tpm_jc *tpm_add_job(t_cm_tpm *tpm, t_cm_work work, void *arg) {
	t_cm_tpm_jc *j;
	bool         r;

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
// not implemented yet
// t_cm_tpm_jc *tpm_add_reccuring_job(t_cm_tpm *tpm, t_cm_work work, void *arg)
// { 	t_cm_tpm_jc *j; 	bool           r;
//
// 	if (!tpm || !work) {
// 		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
// 		return (NULL);
// 	}
// 	j = tpmi_create_job(work, arg);
// 	if (!j) {
// 		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_EALLOC)
// 		return (NULL);
// 	}
// 	j->job.infos.reccuring = true;
// 	pthread_mutex_lock(&tpm->q_lock);
// 	r = tpmi_job_push(j, &tpm->q_start, &tpm->q_end,
// 	                  tpm->internal_val.queue_closed);
// 	pthread_mutex_unlock(&tpm->q_lock);
// 	if (!r) return (free(j), NULL);
// 	pthread_cond_signal(&tpm->q_cond);
// 	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
// 	return (j);
// }
/**
 * \brief enqueue a batch of nb jobs sharing the same work function
 * \param tpm thread pool manager to enqueue the jobs in
 * \param nb number of jobs to create
 * \param work function executed by every job of the batch
 * \param args array of nb arguments, args[i] is passed to the i-th job
 * \return always NULL; the batch is enqueued in order. A failed allocation
 * returns NULL without leaving any partially built batch behind
 * \note if the pool is shutting down the batch is rejected
 * \ingroup tpm
**/
t_cm_tpm_jc *tpm_add_batch(t_cm_tpm *tpm, size_t nb, t_cm_work work,
                           void **args) {
	t_cm_tpm_jc *local_head;
	t_cm_tpm_jc *it;
	bool         shuttingdown;

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
	if (tpm->q_start) tpm->q_start->prev = it;
	if (!tpm->q_end) tpm->q_end = it;
	tpm->q_start = local_head;
	pthread_mutex_unlock(&tpm->q_lock);
	pthread_cond_signal(&tpm->q_cond);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
	return (NULL);
}

/**
 * \brief free a chain of job containers
 * \param head first job container of the chain to free
 * \param cleanup callback called on each job argument before it is freed, may
 * be NULL
 * \ingroup tpm
**/
void tpm_clear_job_list(t_cm_tpm_jc *head, t_cm_call cleanup) {
	for (t_cm_tpm_jc *nxt; head; head = nxt) {
		nxt = head->next;
		if (cleanup) cleanup(head->job.arg);
		free(head);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
}
