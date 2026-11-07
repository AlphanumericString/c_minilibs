#include <assert.h>
#include <stdlib.h>
#define C_MINI__TPM_INTERNAL
#include "thread_pool_manager.h"

t_cm_tpm_jc *tpmi_create_job(t_cm_work w_func, void *a) {
	t_cm_tpm_jc *container;

	container = malloc(sizeof(*container));
	if (!container) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__TPM_LOG_EALLOC)
		return (NULL);
	}
	container->job =
	    (t_cm_tpm_job){.func = w_func, .arg = a, .infos = {.reccuring = false}};
	container->next = NULL;
	container->prev = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
	return (container);
}

bool tpmi_job_push(t_cm_tpm_jc *to_push, t_cm_tpm_jc **head, t_cm_tpm_jc **tail,
                   bool queue_down) {
#ifndef C_MINI__TESTS
	// sanity check: slow's exec speed so left only for dev/tests
	assert(!(*head == NULL ^ *tail == NULL));
#endif
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
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
	return (true);
}
t_cm_tpm_jc tpmi_job_pop(t_cm_tpm *tpm) {
	t_cm_tpm_jc *node;
	t_cm_tpm_jc  ret;

	if (!tpm->q_end) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__TPM_LOG_EARG)
		return ((t_cm_tpm_jc){0});
	}
	node       = tpm->q_end;
	tpm->q_end = tpm->q_end->prev;
	if (node == tpm->q_start) tpm->q_start = NULL;
	ret = *node;
	// if (node->job.infos.reccuring == true) {
	// 	CM_LOG(C_MINI__LEVEL_INFO, "recurring job, re-pushing:%p", node)
	// 	tpmi_job_push(node, &tpm->q_start, &tpm->q_end,
	// 	              tpm->internal_val.queue_closed);
	// } else {
	free(node);
	// }
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__TPM_LOG_SUCCESS)
	return (ret);
}
