#include "intrusive_doubly_linked.h"

/**
 * \brief Reverses the order of nodes in the list.
 * \param head Pointer to the head of the list.
 * \ingroup idl
**/
void idl_reverse(t_cm_idl **head) {
	t_cm_idl *temp;
	t_cm_idl *curr;

	if (!head || !*head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return;
	}
	curr = *head;
	while (curr) {
		temp       = curr->prev;
		curr->prev = curr->next;
		curr->next = temp;
		*head      = curr;
		curr       = curr->prev;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
}
