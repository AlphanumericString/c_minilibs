#include "intrusive_doubly_linked.h"

/**
 * \brief Concatenates two lists by linking the tail of the first to the second.
 * \param list1 The head of the first list.
 * \param list2 The head of the second list.
 * \return The head of the concatenated list.
 * \note no allocation is performed. list2 is spliced as is; the caller must
 * stop using its old head as a separate list.
 * \ingroup idl
**/
t_cm_idl *idl_concat_node(t_cm_idl *list1, t_cm_idl *list2) {
	t_cm_idl *tail;

	if (!list1) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (list2);
	}
	if (!list2) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (list1);
	}
	tail        = idl_get_tail_node(list1);
	tail->next  = list2;
	list2->prev = tail;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (list1);
}

/**
 * \brief Concatenates two element lists by linking the tail of the first to the
 * second.
 * \param list1 The first element of the first list, or NULL.
 * \param list2 The first element of the second list, or NULL.
 * \param offset Offset of the node in both element types.
 * \return The first element of the concatenated list, or NULL.
 * \note no allocation is performed. list2 is spliced as is; the caller must
 * stop using its old head as a separate list.
 * \ingroup idl
**/
void *idl_concat(const void *list1, const void *list2, size_t offset) {
	const t_cm_idl *ret;

	ret = idl_concat_node(list1 ? idl_eton(list1, offset) : NULL,
	                      list2 ? idl_eton(list2, offset) : NULL);
	return (ret ? idl_ntoe(ret) : NULL);
}

/**
 * \brief Alias of idl_sorted_merge() for api naming completeness.
 * \param list1 The head of the first sorted list.
 * \param list2 The head of the second sorted list.
 * \param comp The comparator function.
 * \return The head of the merged sorted list.
 * \ingroup idl
**/
t_cm_idl *idl_merge(t_cm_idl *list1, t_cm_idl *list2, t_cm_comparator comp) {
	return (idl_sorted_merge(list1, list2, comp));
}
