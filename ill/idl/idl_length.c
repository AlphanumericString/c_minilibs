#include "intrusive_doubly_linked.h"

/**
 * \brief Calculates the total length of the list.
 * \param head The head of the list.
 * \return The number of nodes in the list.
 * \ingroup idl
**/
size_t idl_length(t_idl *head) {
	size_t len = 0;
	while (head) {
		len++;
		head = head->next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return len;
}

/**
 * \brief Calculates the length of the list up to a maximum of n nodes.
 * \param head The head of the list.
 * \param n The maximum number of nodes to count.
 * \return The bounded length.
 * \ingroup idl
**/
size_t idl_nlength(t_idl *head, size_t n) {
	size_t len = 0;

	while (head && n) {
		len++;
		head = head->next;
		n--;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return len;
}

/**
 * \brief Counts the number of elements that satisfy the matcher condition.
 * \param head The head of the list.
 * \param matcher The callback used to test elements.
 * \param arg Argument to pass to the matcher.
 * \return The number of matching elements.
 * \ingroup idl
**/
size_t idl_match_nb(t_idl *head, t_cm_callback matcher, void *arg) {
	size_t count = 0;

	if (!head || !matcher) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return count;
	}
	while (head) {
		if (matcher(idl_ntoe(head), arg)) count++;
		head = head->next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return count;
}
