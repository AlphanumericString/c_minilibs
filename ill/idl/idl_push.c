#include "intrusive_doubly_linked.h"

/**
 * \brief Inserts a node at the front of the list.
 * \param head Pointer to the head of the list.
 * \param node The node to insert.
 * \return The new head of the list.
**/
t_idl *idl_push(t_idl **head, t_idl *node) {
	node->next = *head;
	*head      = node;
	return (*head);
}
/**
 * \brief Inserts an entire list of nodes at the front of the list.
 * \param head Pointer to the head of the list.
 * \param nodes The head of the list of nodes to insert.
 * \return The new head of the list.
**/
t_idl *idl_npush(t_idl **head, t_idl *nodes) {
	idl_get_tail(nodes)->next = *head;
	*head                     = nodes;
	return (*head);
}
/**
 * \brief Inserts a node at the end of the list.
 * \param head Pointer to the head of the list.
 * \param node The node to insert.
 * \return The head of the list.
**/
t_idl *idl_push_back(t_idl **head, t_idl *node) {
	if (!*head) return idl_push(head, node);
	idl_get_tail(*head)->next = node;
	node->next                = NULL;
	return (*head);
}

