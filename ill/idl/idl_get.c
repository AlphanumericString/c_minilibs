#include "intrusive_doubly_linked.h"

/**
 * \brief Gets the minimum element in the list according to the comparator.
 * \param head The head of the list.
 * \param c The comparator function used to determine the minimum.
 * \return A pointer to the minimum element, or NULL if the list is empty.
 **/
void *idl_get_min(t_idl *head, t_comparator c) {
	t_idl *curr;
	t_idl *min;

	if (!head || !c) {
		LOG(C_MINI__IDL_LOG_EARG);
		return NULL;
	}
	curr = head;
	min  = head;
	while (curr->next) {
		if (c(idl_ntoe(curr->next), idl_ntoe(min)) < 0) min = curr->next;
		curr = curr->next;
	}
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return idl_ntoe(min);
}

/**
 * \brief Gets the maximum element in the list according to the comparator.
 * \param head The head of the list.
 * \param c The comparator function used to determine the maximum.
 * \return A pointer to the maximum element, or NULL if the list is empty.
 **/
void *idl_get_max(t_idl *head, t_comparator c) {
	t_idl *curr;
	t_idl *max;

	if (!head || !c) {
		LOG(C_MINI__IDL_LOG_EARG);
		return NULL;
	}
	curr = head;
	max  = head;
	while (curr->next) {
		if (c(idl_ntoe(curr->next), idl_ntoe(max)) > 0) max = curr->next;
		curr = curr->next;
	}
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return idl_ntoe(max);
}

/**
 * \brief Gets the k-th element in the list (0-indexed).
 * \param head The head of the list.
 * \param k The index of the element to retrieve.
 * \return A pointer to the k-th element, or NULL if the index is out of bounds.
 **/
void *idl_get_kth(t_idl *head, size_t k) {
	t_idl *curr;

	if (!head) {
		LOG(C_MINI__IDL_LOG_EARG);
		return NULL;
	}

	curr = head;
	while (curr->next && k > 0) {
		curr = curr->next;
		k--;
	}
	if (k > 0) {
		LOG(C_MINI__IDL_LOG_FAIL);
		return NULL;
	}
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return idl_ntoe(curr);
}
