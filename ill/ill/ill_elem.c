#include "intrusive_linked_lists.h"

/**
 * \brief Initializes the node embedded in an element.
 * \param elem Pointer to the user's element structure.
 * \param offset Offset of the node within the element.
 * \ingroup ill
**/
void ill_init(void *elem, size_t offset) {
	ill_init_node(ill_eton(elem, offset), offset);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
}

/**
 * \brief Checks if the node of an element is isolated.
 * \param elem Pointer to the user's element structure.
 * \param offset Offset of the node within the element.
 * \return true if isolated, false otherwise.
 * \ingroup ill
**/
bool ill_is_isolated(const void *elem, size_t offset) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return ill_is_isolated_node(ill_eton(elem, offset));
}

/**
 * \brief Inserts an element immediately after an anchor element.
 * \param anchor_elem Pointer to the anchor element.
 * \param elem Pointer to the element to insert.
 * \param offset Offset of the node within the elements.
 * \ingroup ill
**/
void ill_append(void *anchor_elem, void *elem, size_t offset) {
	ill_append_node(ill_eton(anchor_elem, offset), ill_eton(elem, offset));
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
}

/**
 * \brief Appends an entire element chain after an anchor element.
 * \param anchor_elem Pointer to the anchor element.
 * \param elems Pointer to the first element of the chain to insert.
 * \param offset Offset of the node within the elements.
 * \ingroup ill
**/
void ill_nappend(void *anchor_elem, void *elems, size_t offset) {
	ill_nappend_node(ill_eton(anchor_elem, offset), ill_eton(elems, offset));
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
}

/**
 * \brief Inserts an entire element chain at the front of the list.
 * \param head Pointer to the head of the list.
 * \param elems Pointer to the first element of the chain to insert.
 * \param offset Offset of the node within the elements.
 * \return The new head element, or NULL on failure.
 * \ingroup ill
**/
void *ill_npush(t_cm_ill **head, void *elems, size_t offset) {
	t_cm_ill *ret = ill_npush_node(head, ill_eton(elems, offset));

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Inserts an element at the front of the list.
 * \param head Pointer to the head of the list.
 * \param elem Pointer to the element to insert.
 * \param offset Offset of the node within the element.
 * \return The new head element, or NULL on failure.
 * \ingroup ill
**/
void *ill_push(t_cm_ill **head, void *elem, size_t offset) {
	t_cm_ill *ret = ill_push_node(head, ill_eton(elem, offset));

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Inserts an element at the end of the list.
 * \param head Pointer to the head of the list.
 * \param elem Pointer to the element to insert.
 * \param offset Offset of the node within the element.
 * \return The head element, or NULL on failure.
 * \ingroup ill
**/
void *ill_push_back(t_cm_ill **head, void *elem, size_t offset) {
	t_cm_ill *ret = ill_push_back_node(head, ill_eton(elem, offset));

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Removes and returns the head element of the list.
 * \param head Pointer to the head of the list.
 * \return The removed head element, or NULL if the list was empty.
 * \ingroup ill
**/
void *ill_pop(t_cm_ill **head) {
	t_cm_ill *ret = ill_pop_node(head);

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Removes and returns the tail element of the list.
 * \param head Pointer to the head of the list.
 * \return The removed tail element, or NULL if the list was empty.
 * \ingroup ill
**/
void *ill_pop_back(t_cm_ill **head) {
	t_cm_ill *ret = ill_pop_back_node(head);

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Removes the element immediately following an anchor element.
 * \param prev_elem Pointer to the anchor element.
 * \param offset Offset of the node within the elements.
 * \return The removed element, or NULL if the anchor has no successor.
 * \ingroup ill
**/
void *ill_remove_after(void *prev_elem, size_t offset) {
	t_cm_ill *ret = ill_remove_after_node(ill_eton(prev_elem, offset));

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Removes a specific element from the list.
 * \param head Pointer to the head of the list.
 * \param elem Pointer to the element to remove.
 * \param offset Offset of the node within the element.
 * \return true if the element was successfully removed, false otherwise.
 * \ingroup ill
**/
bool ill_remove(t_cm_ill **head, const void *elem, size_t offset) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return ill_remove_node(head, ill_eton(elem, offset));
}

/**
 * \brief Finds the element immediately preceding a target element.
 * \param head The head of the list.
 * \param elem Pointer to the target element.
 * \param offset Offset of the node within the elements.
 * \return A pointer to the preceding element, or NULL if not found.
 * \ingroup ill
**/
void *ill_find_prev(const t_cm_ill *head, const void *elem, size_t offset) {
	t_cm_ill *ret = ill_find_prev_node(head, ill_eton(elem, offset));

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Finds a specific element in the list.
 * \param head The head of the list.
 * \param elem Pointer to the target element.
 * \param offset Offset of the node within the elements.
 * \return A pointer to the target element if found, or NULL otherwise.
 * \ingroup ill
**/
void *ill_find(const t_cm_ill *head, const void *elem, size_t offset) {
	t_cm_ill *ret = ill_find_node(head, ill_eton(elem, offset));

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Finds the element whose successor's element matches the condition.
 * \param head The head of the list.
 * \param c The comparator function used for matching.
 * \param arg The argument to pass to the comparator.
 * \return A pointer to the preceding element of the matched element, or NULL.
 * \ingroup ill
**/
void *ill_get_prev(const t_cm_ill *head, t_cm_comparator c, const void *arg) {
	t_cm_ill *ret = ill_get_prev_node(head, c, arg);

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Finds the first element matching the given condition.
 * \param head The head of the list.
 * \param c The comparator function used for matching.
 * \param arg The argument to pass to the comparator.
 * \return A pointer to the matched element, or NULL if not found.
 * \ingroup ill
**/
void *ill_get(const t_cm_ill *head, t_cm_comparator c, const void *arg) {
	t_cm_ill *ret = ill_get_node(head, c, arg);

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Retrieves the tail element of the list.
 * \param head The head of the list.
 * \return A pointer to the last element, or NULL if the list is empty.
 * \ingroup ill
**/
void *ill_get_tail(const t_cm_ill *head) {
	t_cm_ill *ret = ill_get_tail_node(head);

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Gets the minimum element in the list according to the comparator.
 * \param head The head of the list.
 * \param comp The comparator function used to determine the minimum.
 * \return A pointer to the minimum element, or NULL if the list is empty.
 * \ingroup ill
**/
void *ill_get_min(const t_cm_ill *head, t_cm_comparator comp) {
	t_cm_ill *ret = ill_get_min_node(head, comp);

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Gets the maximum element in the list according to the comparator.
 * \param head The head of the list.
 * \param comp The comparator function used to determine the maximum.
 * \return A pointer to the maximum element, or NULL if the list is empty.
 * \ingroup ill
**/
void *ill_get_max(const t_cm_ill *head, t_cm_comparator comp) {
	t_cm_ill *ret = ill_get_max_node(head, comp);

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}

/**
 * \brief Gets the k-th element in the list (0-indexed).
 * \param head The head of the list.
 * \param k The index of the element to retrieve.
 * \return A pointer to the k-th element, or NULL if the index is out of bounds.
 * \ingroup ill
**/
void *ill_get_kth(const t_cm_ill *head, size_t k) {
	t_cm_ill *ret = ill_get_kth_node(head, k);

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret ? ill_ntoe(ret) : NULL);
}
