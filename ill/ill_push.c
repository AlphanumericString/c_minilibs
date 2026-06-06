#include "ill.h"

/**
 * \brief Inserts a node at the front of the list.
 * \param head Pointer to the head of the list.
 * \param node The node to insert.
 * \return The new head of the list.
**/
t_ill *ill_push(t_ill **head, t_ill *node) {
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
t_ill *ill_npush(t_ill **head, t_ill *nodes) {
	ill_get_tail(nodes)->next = *head;
	*head                     = nodes;
	return (*head);
}
/**
 * \brief Inserts a node at the end of the list.
 * \param head Pointer to the head of the list.
 * \param node The node to insert.
 * \return The head of the list.
**/
t_ill *ill_push_back(t_ill **head, t_ill *node) {
	if (!*head) return ill_push(head, node);
	ill_get_tail(*head)->next = node;
	node->next                = NULL;
	return (*head);
}
