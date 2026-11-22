#include "intrusive_linked_lists.h"

/**
 * \brief Gets the minimum node in the list according to the comparator.
 * \param head The head of the list.
 * \param c The comparator function used to determine the minimum.
 * \return A pointer to the minimum node, or NULL if the list is empty.
 * \ingroup ill
**/
t_cm_ill *ill_get_min_node(const t_cm_ill *head, t_cm_comparator c) {
	const t_cm_ill *curr;
	const t_cm_ill *min;

	if (!head || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return NULL;
	}
	curr = head;
	min  = head;
	while (curr->next) {
		if (c(ill_ntoe(curr->next), ill_ntoe(min)) < 0) min = curr->next;
		curr = curr->next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (t_cm_ill *)min;
}

/**
 * \brief Gets the maximum node in the list according to the comparator.
 * \param head The head of the list.
 * \param c The comparator function used to determine the maximum.
 * \return A pointer to the maximum node, or NULL if the list is empty.
 * \ingroup ill
**/
t_cm_ill *ill_get_max_node(const t_cm_ill *head, t_cm_comparator c) {
	const t_cm_ill *curr;
	const t_cm_ill *max;

	if (!head || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return NULL;
	}
	curr = head;
	max  = head;
	while (curr->next) {
		if (c(ill_ntoe(curr->next), ill_ntoe(max)) > 0) max = curr->next;
		curr = curr->next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (t_cm_ill *)max;
}

/**
 * \brief Gets the k-th node in the list (0-indexed).
 * \param head The head of the list.
 * \param k The index of the node to retrieve.
 * \return A pointer to the k-th node, or NULL if the index is out of bounds.
 * \ingroup ill
**/
t_cm_ill *ill_get_kth_node(const t_cm_ill *head, size_t k) {
	const t_cm_ill *curr;

	if (!head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return NULL;
	}
	curr = head;
	while (curr->next && k > 0) {
		curr = curr->next;
		k--;
	}
	if (k > 0) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_FAIL)
		return NULL;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (t_cm_ill *)curr;
}
