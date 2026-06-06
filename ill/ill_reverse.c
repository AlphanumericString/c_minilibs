#include "intrusive_linked_lists.h"

/**
 * \brief Reverses the order of nodes in the list.
 * \param head The head of the list.
 * \return The new head of the reversed list.
**/
t_ill *ill_reverse(t_ill *head) {
	t_ill *prev;
	t_ill *curr;
	t_ill *nxt;

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
