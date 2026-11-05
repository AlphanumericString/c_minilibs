#include "intrusive_linked_lists.h"

/**
 * \brief Reverses the order of nodes in the list.
 * \param head The head of the list.
 * \return The new head of the reversed list.
 * \ingroup ill
**/
t_cm_ill *ill_reverse(t_cm_ill *head) {
	t_cm_ill *prev;
	t_cm_ill *curr;
	t_cm_ill *nxt;

	if (!head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	prev = NULL;
	curr = head;
	while (curr) {
		nxt        = curr->next;
		curr->next = prev;
		prev       = curr;
		curr       = nxt;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (prev);
}
