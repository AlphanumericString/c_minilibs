#include "intrusive_linked_lists.h"

/**
 * \brief Removes consecutive elements that the comparator considers equal.
 * \param head The head of the list.
 * \param c The comparator function.
 * \param d Optional destructor called on each removed element.
 * \note Does not check whether the list is sorted, only removes consecutive
 * duplicates.
 * \ingroup ill
**/
void ill_unique(t_ill *head, t_cm_comparator c, t_cm_dtor d) {
	t_ill *dup;

	if (!head || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return;
	}
	while (head->next) {
		if (c(ill_ntoe(head), ill_ntoe(head->next)) != 0) {
			head = head->next;
			continue;
		}
		dup        = head->next;
		head->next = dup->next;
		dup->next  = NULL;
		if (d) d(ill_ntoe(dup));
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
}
