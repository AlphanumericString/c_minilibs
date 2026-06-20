#include "intrusive_doubly_linked.h"

/**
 * \brief Reverses the order of nodes in the list.
 * \param head The head of the list.
 * \return The new head of the reversed list.
**/
t_idl *idl_reverse(t_idl *head) {
	t_idl *temp;
	t_idl *curr;

	if (!head) return (NULL);
	curr = head;
	while (curr) {
		temp       = curr->prev;
		curr->prev = curr->next;
		curr->next = temp;
		head       = curr;
		curr       = curr->prev;
	}
	return (head);
}
