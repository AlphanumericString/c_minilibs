#include "intrusive_doubly_linked.h"

/**
 * \brief Removes the node immediately following the 'prev' node.
 * \param prev The node preceding the one to be removed.
 * \return The removed node, or NULL if 'prev' has no next node.
**/
t_idl *idl_remove_after(t_idl *prev) {
	t_idl *to_remove;

	if (!prev || !prev->next) return NULL;
	to_remove = prev->next;
	prev->next = to_remove->next;
	if (to_remove->next) to_remove->next->prev = prev;
	to_remove->next = NULL;
	to_remove->prev = NULL;
	return to_remove;
}

/**
 * \brief Removes a specific arbitrary node from the list.
 * \param head Pointer to the head of the list.
 * \param target The node to remove.
 * \return true if the node was successfully removed, false otherwise.
**/
bool idl_remove_node(t_idl **head, t_idl *target) {
	if (!*head || !target) return (false);
	if (*head == target) {
		*head = target->next;
		if (*head) (*head)->prev = NULL;
		target->next = NULL;
		target->prev = NULL;
		return (true);
	}
	
	t_idl *curr = idl_find(*head, target);
	if (!curr) return (false);
	
	if (target->prev) target->prev->next = target->next;
	if (target->next) target->next->prev = target->prev;
	target->next = NULL;
	target->prev = NULL;
	return (true);
}
