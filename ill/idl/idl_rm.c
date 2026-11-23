#include "intrusive_doubly_linked.h"

/**
 * \brief Removes the node immediately following the 'prev' node.
 * \param prev The node preceding the one to be removed.
 * \return The removed node, or NULL if 'prev' has no next node.
 * \ingroup idl
**/
t_cm_idl *idl_remove_after_node(t_cm_idl *prev) {
	t_cm_idl *to_remove;

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
 * \note O(1) when target is linked. Define C_MINI__IDL_CHECK_STRICT to also
 * verify membership, at the cost of an O(n) lookup.
 * \ingroup idl
**/
bool idl_remove_node(t_cm_idl **head, t_cm_idl *target) {
#ifdef C_MINI__IDL_CHECK_STRICT
	const t_cm_idl *curr;
#endif

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
#ifdef C_MINI__IDL_CHECK_STRICT
	curr = idl_find_node(*head, target);
	if (!curr) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
		return (false);
	}
#endif

	if (target->prev) target->prev->next = target->next;
	if (target->next) target->next->prev = target->prev;
	target->next = NULL;
	target->prev = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (true);
}
