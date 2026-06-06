#include "intrusive_linked_lists.h"

/**
 * \brief Retrieves the tail (last node) of the list.
 * \param head The head of the list.
 * \return A pointer to the last node in the list, or NULL if the list is empty.
**/
t_ill *ill_get_tail(t_ill *head) {
	if (!head) return NULL;
	while (head->next)
		head = head->next;
	return (t_ill *)head;
}
