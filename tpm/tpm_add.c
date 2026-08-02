#include <assert.h>
#include <pthread.h>
#include <stddef.h>
#include <stdlib.h>

#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"

void disp_chain(t_tpm_job_container *head) {
	DBG("disp chain from %p", head);
	DBG("previous\tit\tnext");
	for (t_tpm_job_container *it = head; it; it = it->next) {
		DBG("%p\t%p\t%p", it->prev, it, it->next);
	}
}

static t_tpm_job_container *create_job(void *(func)(void *), void *a) {
	t_tpm_job_container *container;

	container = malloc(sizeof(*container));
	if (!container) return (NULL);
	container->job = (t_tpm_base_job){
	    .func  = func,
	    .arg   = a,
	    .infos = {.then_function = NULL, .reccuring = false}
    };
	container->next = NULL;
	container->prev = NULL;
	return (container);
}
void tpmi_job_push(t_tpm_job_container *to_push, t_tpm_job_container **head,
                   t_tpm_job_container **tail, bool queue_down) {
	assert(!(*head == NULL ^ *tail == NULL));
	if (queue_down == true) {
		LOG("Queue Down : refused");
		return;
	}
	to_push->next = *head;
	to_push->prev = NULL;
	if (*head) {
		(*head)->prev = to_push;
	} else {
		*tail = to_push;
	}
	*head = to_push;
}

void tpm_add_job(t_tpm *tpm, void *(func)(void *arg), void *arg) {
	t_tpm_job_container *j;

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
	tpmi_job_push(j, &tpm->q_start, &tpm->q_end,
	              tpm->internal_val.queue_closed);
	pthread_mutex_unlock(&tpm->q_lock);
	pthread_cond_signal(&tpm->q_cond);
	LOG(C_MINI__TPM_LOG_SUCCESS);
	return;
}
void tpm_add_reccuring_job(t_tpm *tpm, void *(func)(void *), void *arg) {
	t_tpm_job_container *j;

	if (!tpm || !func) {
		LOG(C_MINI__TPM_LOG_EARG);
		return;
	}
	j = create_job(func, arg);
	if (!j) {
		LOG(C_MINI__TPM_LOG_EALLOC);
		return;
	}
	j->job.infos.reccuring = true;
	pthread_mutex_lock(&tpm->q_lock);
	tpmi_job_push(j, &tpm->q_start, &tpm->q_end,
	              tpm->internal_val.queue_closed);
	pthread_mutex_unlock(&tpm->q_lock);
	pthread_cond_signal(&tpm->q_cond);
	LOG(C_MINI__TPM_LOG_SUCCESS);
	return;
}

void tpm_clear_job_list(t_tpm_job_container *head, void (*arg_freeer)(void *)) {
	for (t_tpm_job_container *nxt; head; head = nxt) {
		nxt = head->next;
		if (arg_freeer) arg_freeer(head->job.arg);
		free(head);
	}
}

void tpm_add_batch(t_tpm *tpm, size_t nb, void *(func)(void *arg),
                   void **args) {
	t_tpm_job_container *local_head;
	t_tpm_job_container *it;

	if (!tpm || !func || !args) {
		LOG(C_MINI__TPM_LOG_EARG);
		return;
	}
	if (!nb) {
		LOG(C_MINI__TPM_LOG_SUCCESS);
		return;
	}
	local_head = create_job(func, args[0]);
	it         = local_head;
	for (size_t i = 1; i < nb; i++) {
		it->next = create_job(func, args[i]);
		if (!it->next) {
			LOG(C_MINI__TPM_LOG_EALLOC);
			tpm_clear_job_list(local_head, NULL);
			return;
		}
		it->next->prev = it;
		it             = it->next;
	}
	pthread_mutex_lock(&tpm->q_lock);
	tpm->q_start = local_head;
	tpm->q_end   = it;
	pthread_mutex_unlock(&tpm->q_lock);
	return;
}

t_tpm_job_container tpmi_job_pop(t_tpm *tpm) {
	t_tpm_job_container *ret;

	if (!tpm->q_end) {
		LOG(C_MINI__TPM_LOG_EARG);
		return ((t_tpm_job_container){0});
	}
	ret        = tpm->q_end;
	tpm->q_end = tpm->q_end->prev;
	if (ret == tpm->q_start) tpm->q_start = NULL;
	ret->prev = NULL;
	ret->next = NULL;
	if (ret->job.infos.reccuring == true) {
		LOG("recuring job, re-pushing:%p", ret);
		tpmi_job_push(ret, &tpm->q_start, &tpm->q_end,
		              tpm->internal_val.queue_closed);
	}
	return (*ret);
}
