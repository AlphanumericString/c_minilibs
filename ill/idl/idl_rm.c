#include "intrusive_doubly_linked.h"

/**
 * \brief Removes the node immediately following the 'prev' node.
 * \param prev The node preceding the one to be removed.
 * \return The removed node, or NULL if 'prev' has no next node.
 * \ingroup idl
**/
t_idl *idl_remove_after(t_idl *prev) {
	t_idl *to_remove;

	if (!prev || !prev->next) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return NULL;
	}
	to_remove  = prev->next;
	prev->next = to_remove->next;
	if (to_remove->next) to_remove->next->prev = prev;
	to_remove->next = NULL;
	to_remove->prev = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return to_remove;
}

/**
 * \brief Removes a specific arbitrary node from the list.
 * \param head Pointer to the head of the list.
 * \param target The node to remove.
 * \return true if the node was successfully removed, false otherwise.
 * \ingroup idl
**/
bool idl_remove_node(t_idl **head, t_idl *target) {
	if (!head || !target || !*head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return false;
	}
	if (*head == target) {
		*head = target->next;
		if (*head) (*head)->prev = NULL;
		target->next = NULL;
		target->prev = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (true);
	}

	t_idl *curr = idl_find(*head, target);
	if (!curr) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
		return (false);
	}

	if (target->prev) target->prev->next = target->next;
	if (target->next) target->next->prev = target->prev;
	target->next = NULL;
	target->prev = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (true);
}
