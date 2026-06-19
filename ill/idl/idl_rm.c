#include "intrusive_doubly_linked.h"

/**
 * \brief Removes the node immediately following the 'prev' node.
 * \param prev The node preceding the one to be removed.
 * \return The removed node, or NULL if 'prev' has no next node.
**/
t_idl *idl_remove_after(t_idl *prev) {
	t_idl *to_remove;

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
bool idl_remove_node(t_idl **head, t_idl *target) {
	t_idl *prev;

	if (!*head || !target) return (false);
	if (*head == target) {
		*head        = target->next;
		target->next = NULL;
		return (true);
	}
	prev = idl_find(*head, target);
	if (prev) {
		prev->next   = target->next;
		target->next = NULL;
		return (true);
	}
	return (false);
}
