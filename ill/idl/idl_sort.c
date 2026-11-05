#include "intrusive_doubly_linked.h"

/**
 * \brief Merges two sorted lists into a single sorted list.
 * \param list1 The head of the first sorted list.
 * \param list2 The head of the second sorted list.
 * \param comp The comparator function.
 * \return The head of the merged sorted list.
 * \ingroup idl
**/
t_cm_idl *idl_sorted_merge(t_cm_idl *list1, t_cm_idl *list2,
                           t_cm_comparator comp) {
	t_cm_idl  dummy;
	t_cm_idl *tail;

	if (!comp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	dummy.prev = NULL;
	tail       = &dummy;
	while (list1 && list2) {
		if (comp(idl_ntoe(list1), idl_ntoe(list2)) <= 0) {
			tail->next  = list1;
			list1->prev = tail;
			list1       = list1->next;
		} else {
			tail->next  = list2;
			list2->prev = tail;
			list2       = list2->next;
		}
		tail = tail->next;
	}
	tail->next = list1 ? list1 : list2;
	if (tail->next) tail->next->prev = tail;
	if (dummy.next) dummy.next->prev = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return dummy.next;
}

static t_cm_idl *split_list(t_cm_idl *head) {
	t_cm_idl *slow;
	t_cm_idl *fast;
	t_cm_idl *mid;

	slow = head;
	fast = head->next;
	while (fast && fast->next) {
		slow = slow->next;
		fast = fast->next->next;
	}
	mid        = slow->next;
	slow->next = NULL;
	mid->prev  = NULL;
	return mid;
}

static t_cm_idl *merge_sort(t_cm_idl *head, t_cm_comparator comp) {
	t_cm_idl *mid;

	if (!head->next) return head;
	mid  = split_list(head);
	head = merge_sort(head, comp);
	mid  = merge_sort(mid, comp);
	return idl_sorted_merge(head, mid, comp);
}

/**
 * \brief Sorts the list in place using a classic merge sort.
 * \param head Pointer to the head of the list.
 * \param comp The comparator function.
 * \ingroup idl
**/
void idl_sort(t_cm_idl **head, t_cm_comparator comp) {
	if (!head || !*head || !comp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return;
	}
	*head         = merge_sort(*head, comp);
	(*head)->prev = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
}
