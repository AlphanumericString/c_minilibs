#include <pthread.h>
#include <signal.h>
#include <stdlib.h>

#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"

static t_tpm_job *create_job(void *(func)(void *), void *a) {
	t_tpm_job *jb;

	jb = malloc(sizeof(*jb));
	if (!jb) return (NULL);
	jb->job  = (t_tpm_base_job){.func = func, .arg = a, .infos = {0}};
	jb->next = NULL;
	jb->prev = NULL;
	return (jb);
}

static void tpmi_job_push(t_tpm_job *to_push, t_tpm_job **head,
                          t_tpm_job **tail) {
	if (head == NULL ^ tail == NULL) {
		LOG("JOB QUEUE DESYNC!! big problems!");
		raise(SIGABRT); // gtfo something is very wrong here
		return;
	}
	to_push->next = *head;
	to_push->prev = NULL;
	if (!head) {
		*head = to_push;
		*tail = to_push;
	} else {
		(*head)->prev = to_push;
	}
}

void tpm_add_job(t_tpm *tpm, void *(func)(void *arg), void *arg) {
	t_tpm_job *j;

	if (!tpm || !func) {
		LOG(C_MINI__TPM_LOG_EARG);
		return;
	}
	j = create_job(func, arg);
	if (!j) {
		LOG(C_MINI__TPM_LOG_EALLOC);
		return;
	}
	pthread_mutex_lock(&tpm->q_lock);
	tpmi_job_push(j, &tpm->q_start, &tpm->q_end);
	pthread_mutex_unlock(&tpm->q_lock);
	pthread_cond_signal(&tpm->q_cond);
	LOG(C_MINI__TPM_LOG_SUCCESS);
	return;
}
void tpm_add_reccuring_job(t_tpm *tpm, void *(func)(void *), void *arg) {
	t_tpm_job *j;

	if (!tpm || !func) {
		LOG(C_MINI__TPM_LOG_EARG);
		return;
	}
	j = create_job(func, arg);
	if (!j) {
		LOG(C_MINI__TPM_LOG_EALLOC);
		return;
	}
	j->job.infos.detail.reccuring = true;
	pthread_mutex_lock(&tpm->q_lock);
	tpmi_job_push(j, &tpm->q_start, &tpm->q_end);
	pthread_mutex_unlock(&tpm->q_lock);
	pthread_cond_signal(&tpm->q_cond);
	LOG(C_MINI__TPM_LOG_SUCCESS);
	return;
}
void tpm_add_jobs(t_tpm *tpm, size_t nb, void *(*funcs)(void *arg),
                  void **args) {
	t_tpm_job *local_head;
	t_tpm_job *it;

	if (!tpm || !funcs || !args) {
		LOG(C_MINI__TPM_LOG_EARG);
		return;
	}
	if (!nb) {
		LOG(C_MINI__TPM_LOG_SUCCESS);
		return;
	}
	(void)local_head;
	(void)it;
	LOG(C_MINI__TPM_LOG_ETODO);
	return;
}
