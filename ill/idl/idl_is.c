#include "intrusive_doubly_linked.h"

/**
 * \brief Checks if a node is isolated (not pointing to any other node).
 * \param node The node to check.
 * \return true if isolated, false otherwise.
 * \ingroup idl
**/
bool idl_is_isolated(t_idl *node) {
	if (!node) {
		LOG(C_MINI__IDL_LOG_EARG);
		return false;
	}
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return node->next == NULL && node->prev == NULL;
}

/**
 * \brief Detects if the list contains a cycle using Floyd's cycle-finding
 * algorithm.
 * \param head The head of the list.
 * \return true if a cycle is detected, false otherwise.
 * \ingroup idl
**/
bool idl_is_circular(t_idl *head) {
	t_idl *fast;
	t_idl *slow;

	if (!head) {
		LOG(C_MINI__IDL_LOG_EARG);
		return false;
	}
	slow = head;
	fast = head;
	while (fast) {
		if (fast->next) fast = fast->next->next;
		else fast = fast->next;
		slow = slow->next;
		if (slow == fast) {
			LOG(C_MINI__IDL_LOG_SUCCESS);
			return (true);
		}
	}
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return (false);
}

/**
 * \brief Checks if the list is sorted according to the given comparator.
 *        Automatically detects ascending or descending order.
 * \param head The head of the list.
 * \param comparator The comparator function.
 * \return true if the list is sorted, false otherwise.
 * \ingroup idl
**/
bool idl_is_sorted(t_idl *head, t_comparator comparator) {
	void *p;
	int   prev;
	int   ord;

	if (!head || !comparator) {
		LOG(C_MINI__IDL_LOG_EARG);
		return (false);
	}
	if (!head->next) return (true);
	prev = 0;
	while (head && head->next) {
		p   = idl_ntoe(head);
		ord = comparator(p, idl_ntoe(head->next));
		if (prev <= 0 && ord < 0) prev = -1;
		else if (prev >= 0 && ord > 0) prev = 1;
		else if (ord != 0) {
			LOG(C_MINI__IDL_LOG_FAIL);
			return (false);
		}
		head = head->next;
	}
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return (true);
}

/**
 * \brief Checks if any element in the list satisfies the matcher condition.
 * \param head The head of the list.
 * \param matcher The callback used to test elements.
 * \param arg Argument to pass to the matcher.
 * \return true if at least one element matches, false otherwise.
 * \ingroup idl
**/
bool idl_has(t_idl *head, t_callback matcher, void *arg) {
	if (!head || !matcher) {
		LOG(C_MINI__IDL_LOG_EARG);
		return (false);
	}
	while (head) {
		if (matcher(idl_ntoe(head), arg)) {
			LOG(C_MINI__IDL_LOG_SUCCESS);
			return (true);
		}
		head = head->next;
	}
	LOG(C_MINI__IDL_LOG_FAIL);
	return (false);
}
