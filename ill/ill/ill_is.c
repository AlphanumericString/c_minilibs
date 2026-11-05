#include "intrusive_linked_lists.h"

/**
 * \brief Checks if a node is isolated (not pointing to any other node).
 * \param node The node to check.
 * \return true if isolated, false otherwise.
 * \ingroup ill
**/
bool ill_is_isolated(const t_cm_ill *node) {
	if (!node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (false);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return node->next == NULL;
}

/**
 * \brief Detects if the list contains a cycle using Floyd's cycle-finding
 * algorithm.
 * \param head The head of the list.
 * \return true if a cycle is detected, false otherwise.
 * \ingroup ill
**/
bool ill_is_circular(const t_cm_ill *head) {
	const t_cm_ill *fast;
	const t_cm_ill *slow;

	if (!head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (false);
	}
	slow = head;
	fast = head;
	while (fast) {
		if (fast->next) fast = fast->next->next;
		else fast = fast->next;
		slow = slow->next;
		if (fast && slow == fast) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
			return (true);
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (false);
}

/**
 * \brief Checks if the list is sorted according to the given comparator.
 *        Automatically detects ascending or descending order.
 * \param head The head of the list.
 * \param comparator The comparator function.
 * \return true if the list is sorted, false otherwise.
 * \ingroup ill
**/
bool ill_is_sorted(const t_cm_ill *head, t_cm_comparator comparator) {
	void *p;
	int   prev;
	int   ord;

	if (!head || !comparator) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (false);
	}
	if (!head->next) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (true);
	}
	prev = 0;
	while (head->next) {
		p   = ill_ntoe(head);
		ord = comparator(p, ill_ntoe(head->next));
		if (prev <= 0 && ord < 0) prev = -1;
		else if (prev >= 0 && ord > 0) prev = 1;
		else if (ord != 0) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_FAIL)
			return (false);
		}
		head = head->next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (true);
}

/**
 * \brief Checks if any element in the list satisfies the matcher condition.
 * \param head The head of the list.
 * \param matcher The callback used to test elements.
 * \param arg Argument to pass to the matcher.
 * \return true if at least one element matches, false otherwise.
 * \ingroup ill
**/
bool ill_has(const t_cm_ill *head, t_cm_callback matcher, void *arg) {
	if (!head || !matcher) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (false);
	}
	while (head) {
		if (matcher(ill_ntoe(head), arg)) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
			return (true);
		}
		head = head->next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_FAIL)
	return (false);
}
