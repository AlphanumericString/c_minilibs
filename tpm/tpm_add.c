#include <assert.h>
#include <pthread.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>

#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"

#define CL     C_MINI__TPM_THRD_JOBS_CACHE
#define t_cont t_tpm_jc

// dbg
void disp_chain(t_cont *head) {
	DBG("disp chain from %p", head);
	DBG("previous\tit\tnext");
	for (t_cont *it = head; it; it = it->next) {
		DBG("%p\t%p\t%p", it->prev, it, it->next);
	}
}

static t_cont *create_job(t_work w_func, void *a) {
	t_cont *container;

	container = malloc(sizeof(*container));
	if (!container) return (NULL);
	container->job =
	    (t_tpm_job){.func = w_func, .arg = a, .infos = {.reccuring = false}};
	container->next = NULL;
	container->prev = NULL;
	return (container);
}

bool tpmi_job_push(t_cont *to_push, t_cont **head, t_cont **tail,
                   bool queue_down) {
	assert(!(*head == NULL ^ *tail == NULL));
	if (queue_down == true) {
		LOG(C_MINI__TPM_LOG_EDOWN);
		return (false);
	}
	to_push->next = *head;
	to_push->prev = NULL;
	if (*head) {
		(*head)->prev = to_push;
	} else {
		*tail = to_push;
	}
	*head = to_push;
	return (true);
}
t_cont tpmi_job_pop(t_tpm *tpm) {
	t_cont *node;
	t_cont  ret;

	if (!tpm->q_end) {
		LOG(C_MINI__TPM_LOG_EARG);
		return ((t_cont){0});
	}
	node       = tpm->q_end;
	tpm->q_end = tpm->q_end->prev;
	if (node == tpm->q_start) tpm->q_start = NULL;
	// node->prev = NULL;
	// node->next = NULL;
	ret = *node;
	if (node->job.infos.reccuring == true) {
		LOG("recuring job, re-pushing:%p", node);
		tpmi_job_push(node, &tpm->q_start, &tpm->q_end,
		              tpm->internal_val.queue_closed);
	} else {
		free(node);
	}
	return (ret);
}

int tpmi_job_thrd_cache_pop(t_tpm *tpm, t_cont *buffer[CL], size_t start) {
	size_t  i = start;
	t_cont *it;

	if (!tpm || !buffer) {
		LOG(C_MINI__TPM_LOG_EARG);
		return (-1);
	}
	it = tpm->q_start;
	for (; i < CL && it; i++) {
		buffer[i] = it;
		it        = it->next;
	}
	if (!it || !it->next) {
		tpm->q_start = NULL;
		tpm->q_end   = NULL;
	} else {
		it->next->prev = NULL;
		tpm->q_start   = it->next;
	}
	LOG("Success: nb jobs taken:%zu", i - start);
	return (i - start);
}

t_cont *tpm_add_job(t_tpm *tpm, t_work work, void *arg) {
	t_cont *j;
	bool    r;

	if (!tpm || !work) {
		LOG(C_MINI__TPM_LOG_EARG);
		return (NULL);
	}
	j = create_job(work, arg);
	if (!j) {
		LOG(C_MINI__TPM_LOG_EALLOC);
		return (NULL);
	}
	pthread_mutex_lock(&tpm->q_lock);
	r = tpmi_job_push(j, &tpm->q_start, &tpm->q_end,
	                  tpm->internal_val.queue_closed);
	pthread_mutex_unlock(&tpm->q_lock);
	if (!r) return (free(j), NULL);
	pthread_cond_signal(&tpm->q_cond);
	LOG(C_MINI__TPM_LOG_SUCCESS);
	return (j);
}
t_cont *tpm_add_reccuring_job(t_tpm *tpm, t_work work, void *arg) {
	t_cont *j;
	bool    r;

	if (!tpm || !work) {
		LOG(C_MINI__TPM_LOG_EARG);
		return (NULL);
	}
	j = create_job(work, arg);
	if (!j) {
		LOG(C_MINI__TPM_LOG_EALLOC);
		return (NULL);
	}
	j->job.infos.reccuring = true;
	pthread_mutex_lock(&tpm->q_lock);
	r = tpmi_job_push(j, &tpm->q_start, &tpm->q_end,
	                  tpm->internal_val.queue_closed);
	pthread_mutex_unlock(&tpm->q_lock);
	if (!r) return (free(j), NULL);
	pthread_cond_signal(&tpm->q_cond);
	LOG(C_MINI__TPM_LOG_SUCCESS);
	return (j);
}
t_cont *tpm_add_batch(t_tpm *tpm, size_t nb, t_work work, void **args) {
	t_cont *local_head;
	t_cont *it;
	bool    shuttingdown;

	if (!tpm || !work || !args) {
		LOG(C_MINI__TPM_LOG_EARG);
		return (NULL);
	}
	if (!nb) {
		LOG(C_MINI__TPM_LOG_SUCCESS);
		return (NULL);
	}
	pthread_mutex_lock(&tpm->q_lock);
	shuttingdown = tpm->internal_val.shutting_down;
	pthread_mutex_unlock(&tpm->q_lock);
	if (shuttingdown) {
		LOG(C_MINI__TPM_LOG_EDOWN);
		return (NULL);
	}
	local_head = create_job(work, args[0]);
	it         = local_head;
	for (size_t i = 1; i < nb; i++) {
		it->next = create_job(work, args[i]);
		if (!it->next) {
			LOG(C_MINI__TPM_LOG_EALLOC);
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

void tpm_clear_job_list(t_cont *head, t_call cleanup) {
	for (t_cont *nxt; head; head = nxt) {
		nxt = head->next;
		if (cleanup) cleanup(head->job.arg);
		free(head);
	}
}
