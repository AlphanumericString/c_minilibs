#include "intrusive_doubly_linked.h"

/**
 * \brief Checks if a node is isolated (not pointing to any other node).
 * \param node The node to check.
 * \return true if isolated, false otherwise.
**/
bool idl_is_isolated(t_idl *node) {
	return node->next == NULL;
}

/**
 * \brief Detects if the list contains a cycle using Floyd's cycle-finding
 * algorithm.
 * \param head The head of the list.
 * \return true if a cycle is detected, false otherwise.
**/
bool idl_is_circular(t_idl *head) {
	t_idl *fast;
	t_idl *slow;

	slow = head;
	fast = head;
	while (fast) {
		if (fast->next) fast = fast->next->next;
		else fast = fast->next;
		slow = slow->next;
		if (slow == fast) return (true);
	}
	return (false);
}

/**
 * \brief Checks if the list is sorted according to the given comparator.
 *        Automatically detects ascending or descending order.
 * \param head The head of the list.
 * \param comparator The comparator function.
 * \return true if the list is sorted, false otherwise.
**/
bool idl_is_sorted(t_idl *head, t_comparator comparator) {
	void *p;
	int   prev;
	int   ord;

	if (!head || !head->next) return (true);
	if (!comparator) return (false);
	prev = 0;
	while (head && head->next) {
		p   = idl_ntoe(head);
		ord = comparator(p, idl_ntoe(head->next));
		if (prev <= 0 && ord < 0) prev = -1;
		else if (prev >= 0 && ord > 0) prev = 1;
		else if (ord != 0) return (false);
		head = head->next;
	}
	return (true);
}

/**
 * \brief Checks if any element in the list satisfies the matcher condition.
 * \param head The head of the list.
 * \param matcher The callback used to test elements.
 * \param arg Argument to pass to the matcher.
 * \return true if at least one element matches, false otherwise.
**/
bool idl_has(t_idl *head, t_callback matcher, void *arg) {
	while (head) {
		if (matcher(idl_ntoe(head), arg)) return (true);
		head = head->next;
	}
	return (false);
}
