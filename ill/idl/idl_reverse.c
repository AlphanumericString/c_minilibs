#include "intrusive_doubly_linked.h"

/**
 * \brief Reverses the order of nodes in the list.
 * \param head The head of the list.
 * \return The new head of the reversed list.
**/
t_idl *idl_reverse(t_idl *head) {
	t_idl *prev;
	t_idl *curr;
	t_idl *nxt;

	if (!head) return (NULL);
	prev = NULL;
	curr = head;
	while (curr) {
		nxt        = curr->next;
		curr->next = prev;
		prev       = curr;
		curr       = nxt;
	}
	return (prev);
}
