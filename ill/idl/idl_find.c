#include "intrusive_doubly_linked.h"

/**
 * \brief Finds the node immediately preceding the target node in the list.
 * \param head The head of the list.
 * \param target The target node to find the predecessor for.
 * \return A pointer to the predecessor node, or NULL if not found or if target
 * is head.
**/
t_idl *idl_find_prev(t_idl *head, t_idl *target) {
	t_idl *curr;

	if (!head || head == target) return NULL;
	curr = head;
	while (curr->next && curr->next != target)
		curr = curr->next;

	return curr->next == target ? curr : NULL;
}

/**
 * \brief Finds a specific node in the list.
 * \param head The head of the list.
 * \param target The target node to find.
 * \return A pointer to the target node if found, or NULL otherwise.
**/
t_idl *idl_find(t_idl *head, t_idl *target) {
	t_idl *curr;

	if (!head || !target) return NULL;

	curr = head;
	while (curr && curr != target) {
		curr = curr->next;
	}
	return curr;
}

/**
 * \brief Finds the node whose successor's element matches the given condition.
 * \param head The head of the list.
 * \param c The comparator function used for matching.
 * \param arg The argument to pass to the comparator.
 * \return A pointer to the preceding node of the matched element, or NULL if
 * not found.
**/
t_idl *idl_get_prev(t_idl *head, t_comparator c, void *arg) {
	t_idl *curr;

	if (!head || !c) return NULL;
	curr = head;
	while (curr->next) {
		if (c(idl_ntoe(curr->next), arg) == 0) return curr;
		curr = curr->next;
	}
	return NULL;
}

/**
 * \brief Finds the first node whose element matches the given condition.
 * \param head The head of the list.
 * \param c The comparator function used for matching.
 * \param arg The argument to pass to the comparator.
 * \return A pointer to the matched node, or NULL if not found.
**/
t_idl *idl_get(t_idl *head, t_comparator c, void *arg) {
	t_idl *curr;

	if (!head || !c) return NULL;
	curr = head;
	while (curr) {
		if (c(idl_ntoe(curr), arg) == 0) return curr;
		curr = curr->next;
	}
	return NULL;
}
