#include <assert.h>
#include <stdlib.h>
#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"

#define t_cont t_tpm_jc

t_cont *tpmi_create_job(t_work w_func, void *a) {
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
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EDOWN)
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
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return ((t_cont){0});
	}
	node       = tpm->q_end;
	tpm->q_end = tpm->q_end->prev;
	if (node == tpm->q_start) tpm->q_start = NULL;
	// node->prev = NULL;
	// node->next = NULL;
	ret = *node;
	if (node->job.infos.reccuring == true) {
		CM_LOG(C_MINI__LEVEL_INFO, "recurring job, re-pushing:%p", node)
		tpmi_job_push(node, &tpm->q_start, &tpm->q_end,
		              tpm->internal_val.queue_closed);
	} else {
		free(node);
	}
	return (ret);
}
