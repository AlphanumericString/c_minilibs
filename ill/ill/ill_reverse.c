#include "intrusive_linked_lists.h"

/**
 * \brief Reverses the order of nodes in the list.
 * \param head Pointer to the head of the list.
 * \ingroup ill
**/
void ill_reverse(t_cm_ill **head) {
	t_cm_ill *prev;
	t_cm_ill *curr;
	t_cm_ill *nxt;

	if (!head || !*head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return;
	}
	prev = NULL;
	curr = *head;
	while (curr) {
		nxt        = curr->next;
		curr->next = prev;
		prev       = curr;
		curr       = nxt;
	}
	*head = prev;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
}
