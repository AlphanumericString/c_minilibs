#include "intrusive_linked_lists.h"

/**
 * \brief Merges two sorted lists into a single sorted list.
 * \param list1 The head of the first sorted list.
 * \param list2 The head of the second sorted list.
 * \param comp The comparator function.
 * \return The head of the merged sorted list.
 * \ingroup ill
**/
t_cm_ill *ill_sorted_merge(t_cm_ill *list1, t_cm_ill *list2,
                           t_cm_comparator comp) {
	t_cm_ill  dummy;
	t_cm_ill *tail;

	if (!comp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	tail = &dummy;
	while (list1 && list2) {
		if (comp(ill_ntoe(list1), ill_ntoe(list2)) <= 0) {
			tail->next = list1;
			list1      = list1->next;
		} else {
			tail->next = list2;
			list2      = list2->next;
		}
		tail = tail->next;
	}
	tail->next = list1 ? list1 : list2;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return dummy.next;
}

static t_cm_ill *split_list(t_cm_ill *head) {
	t_cm_ill *slow;
	t_cm_ill *fast;
	t_cm_ill *mid;

	slow = head;
	fast = head->next;
	while (fast && fast->next) {
		slow = slow->next;
		fast = fast->next->next;
	}
	mid        = slow->next;
	slow->next = NULL;
	return mid;
}

static t_cm_ill *merge_sort(t_cm_ill *head, t_cm_comparator comp) {
	t_cm_ill *mid;

	if (!head->next) return head;
	mid  = split_list(head);
	head = merge_sort(head, comp);
	mid  = merge_sort(mid, comp);
	return ill_sorted_merge(head, mid, comp);
}

/**
 * \brief Sorts the list in place using a classic merge sort.
 * \param head Pointer to the head of the list.
 * \param comp The comparator function.
 * \ingroup ill
**/
void ill_sort(t_cm_ill **head, t_cm_comparator comp) {
	if (!head || !*head || !comp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return;
	}
	*head = merge_sort(*head, comp);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
}
