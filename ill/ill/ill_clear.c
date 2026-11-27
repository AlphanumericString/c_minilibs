#include "intrusive_linked_lists.h"

/**
 * \brief Removes every node from a list, optionally destroying the elements.
 * \param head Pointer to the head of the list.
 * \param dtor Optional destructor called on every element.
 * \note every node is unlinked and isolated before *head is reset to NULL.
 * \ingroup ill
**/
void ill_clear(t_cm_ill **head, t_cm_dtor dtor) {
	t_cm_ill *curr;

	if (!head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return;
	}
	curr = *head;
	while (curr) {
		t_cm_ill *nxt = curr->next;

		curr->next = NULL;
		if (dtor) dtor(ill_ntoe(curr));
		curr = nxt;
	}
	*head = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
}
