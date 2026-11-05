#include "intrusive_doubly_linked.h"

/**
 * \brief Removes consecutive elements that the comparator considers equal.
 * \param head The head of the list.
 * \param c The comparator function.
 * \param d Optional destructor called on each removed element.
 * \note Does not check whether the list is sorted, only removes consecutive
 * duplicates.
 * \ingroup idl
**/
void idl_unique(t_cm_idl *head, t_cm_comparator c, t_cm_dtor d) {
	t_cm_idl *dup;

	if (!head || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return;
	}
	while (head->next) {
		if (c(idl_ntoe(head), idl_ntoe(head->next)) != 0) {
			head = head->next;
			continue;
		}
		dup        = head->next;
		head->next = dup->next;
		if (dup->next) dup->next->prev = head;
		dup->next = NULL;
		dup->prev = NULL;
		if (d) d(idl_ntoe(dup));
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
}
