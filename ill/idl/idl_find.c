#include "intrusive_doubly_linked.h"

/**
 * \brief Finds the node immediately preceding the target node in the list.
 * \param head The head of the list.
 * \param target The target node to find the predecessor for.
 * \return A pointer to the predecessor node, or NULL if target is the head or
 * is not found.
 * \note NULL is ambiguous: use idl_find_node() first to distinguish "head" from
 * "not found".
 * \ingroup idl
**/
t_cm_idl *idl_find_prev_node(const t_cm_idl *head, const t_cm_idl *target) {
	const t_cm_idl *curr;

	if (!head || !target || head == target) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return NULL;
	}
	curr = head;
	while (curr->next && curr->next != target)
		curr = curr->next;
	if (!curr->next) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (t_cm_idl *)curr;
}

/**
 * \brief Finds a specific node in the list.
 * \param head The head of the list.
 * \param target The target node to find.
 * \return A pointer to the target node if found, or NULL otherwise.
 * \ingroup idl
**/
t_cm_idl *idl_find_node(const t_cm_idl *head, const t_cm_idl *target) {
	const t_cm_idl *curr;

	if (!head || !target) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return NULL;
	}
	curr = head;
	while (curr && curr != target)
		curr = curr->next;
	if (!curr) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (t_cm_idl *)curr;
}

/**
 * \brief Finds the node whose successor's element matches the given condition.
 * \param head The head of the list.
 * \param c The comparator function used for matching.
 * \param arg The argument to pass to the comparator.
 * \return A pointer to the preceding node of the matched element, or NULL if
 * there is no match or the match is the head.
 * \note NULL is ambiguous: use idl_get_node() first to distinguish "match is
 * head" from "no match".
 * \ingroup idl
**/
t_cm_idl *idl_get_prev_node(const t_cm_idl *head, t_cm_comparator c,
                            const void *arg) {
	const t_cm_idl *curr;

	if (!head || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return NULL;
	}
	curr = head;
	while (curr->next) {
		if (c(idl_ntoe(curr->next), arg) == 0) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
			return (t_cm_idl *)curr;
		}
		curr = curr->next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
	return (NULL);
}

/**
 * \brief Finds the first node whose element matches the given condition.
 * \param head The head of the list.
 * \param c The comparator function used for matching.
 * \param arg The argument to pass to the comparator.
 * \return A pointer to the matched node, or NULL if not found.
 * \ingroup idl
**/
t_cm_idl *idl_get_node(const t_cm_idl *head, t_cm_comparator c,
                       const void *arg) {
	const t_cm_idl *curr;

	if (!head || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return NULL;
	}
	curr = head;
	while (curr) {
		if (c(idl_ntoe(curr), arg) == 0) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
			return (t_cm_idl *)curr;
		}
		curr = curr->next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
	return NULL;
}
