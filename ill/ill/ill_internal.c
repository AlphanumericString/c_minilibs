#define C_MINI__ILL_INTERNAL
#include "intrusive_linked_lists.h"

/**
 * \brief [INTERNAL] [MAY CHANGE] inserts a node at the back of a singly linked
 * list using a cached tail.
 * \param head Pointer to the head of the list.
 * \param tail Pointer to the cached tail of the list.
 * \param node The node to insert.
 * \return The head of the list, or NULL on invalid arguments.
 * \note this is an INTERNAL interface. to use add a define to
 * C_MINI__ILL_INTERNAL before including intrusive_linked_lists.h.
 * \ingroup ill
**/
t_cm_ill *illi_push_back_node(t_cm_ill **head, t_cm_ill **tail,
                              t_cm_ill *node) {
	if (!head || !tail || !node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	if (!*head) {
		node->next = NULL;
		*head      = node;
		*tail      = node;
	} else {
		(*tail)->next = node;
		node->next    = NULL;
		*tail         = node;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (*head);
}

/**
 * \brief [INTERNAL] [MAY CHANGE] removes the tail of a singly linked list
 * using a cached tail.
 * \param head Pointer to the head of the list.
 * \param tail Pointer to the cached tail of the list.
 * \return The removed node, or NULL if the list was empty.
 * \note this is an INTERNAL interface. to use add a define to
 * C_MINI__ILL_INTERNAL before including intrusive_linked_lists.h.
 * \ingroup ill
**/
t_cm_ill *illi_pop_back_node(t_cm_ill **head, t_cm_ill **tail) {
	t_cm_ill *curr;
	t_cm_ill *ret;

	if (!head || !tail || !*head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	if (*head == *tail) {
		ret       = *head;
		*head     = NULL;
		*tail     = NULL;
		ret->next = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (ret);
	}
	curr = *head;
	while (curr->next != *tail)
		curr = curr->next;
	ret        = *tail;
	curr->next = NULL;
	*tail      = curr;
	ret->next  = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (ret);
}

/**
 * \brief [INTERNAL] [MAY CHANGE] concatenates two singly linked lists using
 * cached heads and tails.
 * \param h1 Pointer to the head of the first list.
 * \param t1 Pointer to the cached tail of the first list.
 * \param h2 Pointer to the head of the second list.
 * \param t2 Pointer to the cached tail of the second list.
 * \return The head of the concatenated list, or NULL on invalid arguments.
 * \note this is an INTERNAL interface. to use add a define to
 * C_MINI__ILL_INTERNAL before including intrusive_linked_lists.h.
 * \ingroup ill
**/
t_cm_ill *illi_concat_node(t_cm_ill **h1, t_cm_ill **t1, t_cm_ill **h2,
                           t_cm_ill **t2) {
	if (!h1 || !t1 || !h2 || !t2) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	if (!*h2) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (*h1);
	}
	if (!*h1) {
		*h1 = *h2;
		*t1 = *t2;
	} else {
		(*t1)->next = *h2;
		*t1         = *t2;
	}
	*h2 = NULL;
	*t2 = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (*h1);
}

/**
 * \brief [INTERNAL] [MAY CHANGE] removes a node from a singly linked list
 * using a cached tail.
 * \param head Pointer to the head of the list.
 * \param tail Pointer to the cached tail of the list.
 * \param target The node to remove.
 * \return The removed node, or NULL if the node was not found.
 * \note this is an INTERNAL interface. to use add a define to
 * C_MINI__ILL_INTERNAL before including intrusive_linked_lists.h.
 * \ingroup ill
**/
t_cm_ill *illi_remove_node(t_cm_ill **head, t_cm_ill **tail, t_cm_ill *target) {
	t_cm_ill *curr;

	if (!head || !tail || !*head || !target) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	if (*head == target) {
		*head = target->next;
		if (!*head) *tail = NULL;
		target->next = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (target);
	}
	curr = *head;
	while (curr->next && curr->next != target)
		curr = curr->next;
	if (!curr->next) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_FAIL)
		return (NULL);
	}
	curr->next = target->next;
	if (*tail == target) *tail = curr;
	target->next = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (target);
}
