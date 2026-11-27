#define C_MINI__IDL_INTERNAL
#include "intrusive_doubly_linked.h"

/**
 * \brief [INTERNAL] [MAY CHANGE] inserts a node at the back of a doubly linked
 * list using a cached tail.
 * \param head Pointer to the head of the list.
 * \param tail Pointer to the cached tail of the list.
 * \param node The node to insert.
 * \return The head of the list, or NULL on invalid arguments.
 * \note this is an INTERNAL interface. to use add a define to
 * C_MINI__IDL_INTERNAL before including intrusive_doubly_linked.h.
 * \ingroup idl
**/
t_cm_idl *idli_push_back_node(t_cm_idl **head, t_cm_idl **tail,
                              t_cm_idl *node) {
	if (!head || !tail || !node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	if (!*head) {
		node->prev = NULL;
		node->next = NULL;
		*head      = node;
		*tail      = node;
	} else {
		node->prev    = *tail;
		node->next    = NULL;
		(*tail)->next = node;
		*tail         = node;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (*head);
}

/**
 * \brief [INTERNAL] [MAY CHANGE] removes the tail of a doubly linked list
 * using a cached tail.
 * \param head Pointer to the head of the list.
 * \param tail Pointer to the cached tail of the list.
 * \return The removed node, or NULL if the list was empty.
 * \note this is an INTERNAL interface. to use add a define to
 * C_MINI__IDL_INTERNAL before including intrusive_doubly_linked.h.
 * \ingroup idl
**/
t_cm_idl *idli_pop_back_node(t_cm_idl **head, t_cm_idl **tail) {
	t_cm_idl *ret;

	if (!head || !tail || !*head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	ret   = *tail;
	*tail = ret->prev;
	if (*tail) (*tail)->next = NULL;
	else *head = NULL;
	ret->prev = NULL;
	ret->next = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (ret);
}

/**
 * \brief [INTERNAL] [MAY CHANGE] concatenates two doubly linked lists using
 * cached heads and tails.
 * \param h1 Pointer to the head of the first list.
 * \param t1 Pointer to the cached tail of the first list.
 * \param h2 Pointer to the head of the second list.
 * \param t2 Pointer to the cached tail of the second list.
 * \return The head of the concatenated list, or NULL on invalid arguments.
 * \note this is an INTERNAL interface. to use add a define to
 * C_MINI__IDL_INTERNAL before including intrusive_doubly_linked.h.
 * \ingroup idl
**/
t_cm_idl *idli_concat_node(t_cm_idl **h1, t_cm_idl **t1, t_cm_idl **h2,
                           t_cm_idl **t2) {
	if (!h1 || !t1 || !h2 || !t2) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	if (!*h2) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (*h1);
	}
	if (!*h1) {
		*h1 = *h2;
		*t1 = *t2;
	} else {
		(*t1)->next = *h2;
		(*h2)->prev = *t1;
		*t1         = *t2;
	}
	*h2 = NULL;
	*t2 = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (*h1);
}

/**
 * \brief [INTERNAL] [MAY CHANGE] removes a node from a doubly linked list
 * using a cached tail.
 * \param head Pointer to the head of the list.
 * \param tail Pointer to the cached tail of the list.
 * \param target The node to remove.
 * \return The removed node, or NULL if the node was not found.
 * \note this is an INTERNAL interface. to use add a define to
 * C_MINI__IDL_INTERNAL before including intrusive_doubly_linked.h.
 * \ingroup idl
**/
t_cm_idl *idli_remove_node(t_cm_idl **head, t_cm_idl **tail, t_cm_idl *target) {
	if (!head || !tail || !*head || !target) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	if (target == *head) {
		*head = target->next;
		if (*head) (*head)->prev = NULL;
		else *tail = NULL;
	} else {
		if (!target->prev) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
			return (NULL);
		}
		target->prev->next = target->next;
		if (target->next) target->next->prev = target->prev;
		if (target == *tail) *tail = target->prev;
	}
	target->prev = NULL;
	target->next = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (target);
}
