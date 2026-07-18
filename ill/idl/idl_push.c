#include "intrusive_doubly_linked.h"

/**
 * \brief Inserts a node at the front of the list.
 * \param head Pointer to the head of the list.
 * \param node The node to insert.
 * \return The new head of the list.
 * \ingroup idl
 **/
t_idl *idl_push(t_idl **head, t_idl *node) {
	if (!head || !node) {
		LOG(C_MINI__IDL_LOG_EARG);
		return (NULL);
	}
	node->next = *head;
	node->prev = NULL;
	if (*head) (*head)->prev = node;
	*head = node;
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return (*head);
}
/**
 * \brief Inserts an entire list of nodes at the front of the list.
 * \param head Pointer to the head of the list.
 * \param nodes The head of the list of nodes to insert.
 * \return The new head of the list.
 * \ingroup idl
 **/
t_idl *idl_npush(t_idl **head, t_idl *nodes) {
	if (!head || !nodes) {
		LOG(C_MINI__IDL_LOG_EARG);
		return (NULL);
	}
	t_idl *tail = idl_get_tail(nodes);
	tail->next  = *head;
	if (*head) (*head)->prev = tail;
	nodes->prev = NULL;
	*head       = nodes;
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return (*head);
}
/**
 * \brief Inserts a node at the end of the list.
 * \param head Pointer to the head of the list.
 * \param node The node to insert.
 * \return The head of the list.
 * \ingroup idl
 **/
t_idl *idl_push_back(t_idl **head, t_idl *node) {
	if (!head || !node) {
		LOG(C_MINI__IDL_LOG_EARG);
		return (NULL);
	}
	if (!*head) {
		idl_push(head, node);
		LOG(C_MINI__IDL_LOG_SUCCESS);
		return (*head);
	}
	t_idl *tail = idl_get_tail(*head);
	tail->next  = node;
	node->prev  = tail;
	node->next  = NULL;
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return (*head);
}
