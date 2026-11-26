#include "intrusive_doubly_linked.h"

/**
 * \brief Removes every node from a list, optionally destroying the elements.
 * \param head Pointer to the head of the list.
 * \param dtor Optional destructor called on every element.
 * \note every node is unlinked on both sides before *head is reset to NULL.
 * \ingroup idl
**/
void idl_clear(t_cm_idl **head, t_cm_dtor dtor) {
	t_cm_idl *curr;

	if (!head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return;
	}
	curr = *head;
	while (curr) {
		t_cm_idl *nxt = curr->next;

		curr->next = NULL;
		curr->prev = NULL;
		if (dtor) dtor(idl_ntoe(curr));
		curr = nxt;
	}
	*head = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
}
