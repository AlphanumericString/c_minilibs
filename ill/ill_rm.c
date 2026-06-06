#include "ill.h"

/**
 * \brief Removes the node immediately following the 'prev' node.
 * \param prev The node preceding the one to be removed.
 * \return The removed node, or NULL if 'prev' has no next node.
**/
t_ill *ill_remove_after(t_ill *prev) {
	t_ill *to_remove;

	if (!prev || !prev->next) return NULL;
	to_remove       = prev->next;
	prev->next      = to_remove->next;
	to_remove->next = NULL;
	return to_remove;
}

/**
 * \brief Removes a specific arbitrary node from the list.
 * \param head Pointer to the head of the list.
 * \param target The node to remove.
 * \return true if the node was successfully removed, false otherwise.
**/
bool ill_remove_node(t_ill **head, t_ill *target) {
	t_ill *prev;

	if (!*head || !target) return false;
	// If removing head
	if (*head == target) {
		*head        = target->next;
		target->next = NULL;
		return true;
	}

	prev = NULL;
	// Find predecessor
	prev = ill_find_prev(*head, target);
	if (prev) {
		prev->next   = target->next;
		target->next = NULL;
		return true;
	}

	return false;
}
