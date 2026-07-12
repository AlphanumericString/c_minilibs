#include "intrusive_linked_lists.h"

/**
 * \brief Finds the node immediately preceding the target node in the list.
 * \param head The head of the list.
 * \param target The target node to find the predecessor for.
 * \return A pointer to the predecessor node, or NULL if not found or if target
 * is head.
 **/
t_ill *ill_find_prev(t_ill *head, t_ill *target) {
	t_ill *curr;

	if (!head || head == target) {
		LOG(C_MINI__ILL_LOG_EARG);
		return NULL;
	}
	curr = head;
	while (curr->next && curr->next != target)
		curr = curr->next;
	if (!curr->next) {
		LOG(C_MINI__ILL_LOG_FAIL);
		return (NULL);
	}
	LOG(C_MINI__ILL_LOG_SUCCESS);
	return curr;
}

/**
 * \brief Finds a specific node in the list.
 * \param head The head of the list.
 * \param target The target node to find.
 * \return A pointer to the target node if found, or NULL otherwise.
 **/
t_ill *ill_find(t_ill *head, t_ill *target) {
	t_ill *curr;

	if (!head || !target) {
		LOG(C_MINI__ILL_LOG_EARG);
		return NULL;
	}

	curr = head;
	while (curr && curr != target) {
		curr = curr->next;
	}
	if (!curr) {
		LOG(C_MINI__ILL_LOG_FAIL);
		return (NULL);
	}
	LOG(C_MINI__ILL_LOG_SUCCESS);
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
t_ill *ill_get_prev(t_ill *head, t_comparator c, void *arg) {
	t_ill *curr;

	if (!head || !c) {
		LOG(C_MINI__ILL_LOG_EARG);
		return NULL;
	}
	curr = head;
	while (curr->next) {
		if (c(ill_ntoe(curr->next), arg) == 0) {
			LOG(C_MINI__ILL_LOG_SUCCESS);
			return curr;
		}
		curr = curr->next;
	}
	LOG(C_MINI__ILL_LOG_FAIL);
	return NULL;
}

/**
 * \brief Finds the first node whose element matches the given condition.
 * \param head The head of the list.
 * \param c The comparator function used for matching.
 * \param arg The argument to pass to the comparator.
 * \return A pointer to the matched node, or NULL if not found.
 **/
t_ill *ill_get(t_ill *head, t_comparator c, void *arg) {
	t_ill *curr;

	if (!head || !c) {
		LOG(C_MINI__ILL_LOG_EARG);
		return NULL;
	}
	curr = head;
	while (curr) {
		if (c(ill_ntoe(curr), arg) == 0) {
			LOG(C_MINI__ILL_LOG_SUCCESS);
			return curr;
		}
		curr = curr->next;
	}
	LOG(C_MINI__ILL_LOG_FAIL);
	return NULL;
}
