#include "intrusive_doubly_linked.h"

/**
 * \brief Finds the node immediately preceding the target node in the list.
 * \param head The head of the list.
 * \param target The target node to find the predecessor for.
 * \return A pointer to the predecessor node, or NULL if not found or if target
 * is head.
 * \ingroup idl
**/
t_idl *idl_find_prev(t_idl *head, t_idl *target) {
	t_idl *curr;

	if (!head || !target || head == target) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG);
		return NULL;
	}
	curr = head;
	while (curr->next && curr->next != target)
		curr = curr->next;
	if (!curr->next) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS);
	return curr;
}

/**
 * \brief Finds a specific node in the list.
 * \param head The head of the list.
 * \param target The target node to find.
 * \return A pointer to the target node if found, or NULL otherwise.
 * \ingroup idl
**/
t_idl *idl_find(t_idl *head, t_idl *target) {
	t_idl *curr;

	if (!head || !target) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG);
		return NULL;
	}
	curr = head;
	while (curr && curr != target)
		curr = curr->next;
	if (!curr) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS);
	return curr;
}

/**
 * \brief Finds the node whose successor's element matches the given condition.
 * \param head The head of the list.
 * \param c The comparator function used for matching.
 * \param arg The argument to pass to the comparator.
 * \return A pointer to the preceding node of the matched element, or NULL if
 * not found.
 * \ingroup idl
**/
t_idl *idl_get_prev(t_idl *head, t_comparator c, void *arg) {
	t_idl *curr;

	if (!head || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG);
		return NULL;
	}
	curr = head;
	while (curr->next) {
		if (c(idl_ntoe(curr->next), arg) == 0) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS);
			return curr;
		}
		curr = curr->next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL);
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
t_idl *idl_get(t_idl *head, t_comparator c, void *arg) {
	t_idl *curr;

	if (!head || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG);
		return NULL;
	}
	curr = head;
	while (curr) {
		if (c(idl_ntoe(curr), arg) == 0) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS);
			return curr;
		}
		curr = curr->next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL);
	return NULL;
}
