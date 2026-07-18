#include "intrusive_linked_lists.h"

/**
 * \brief Inserts a node at the front of the list.
 * \param head Pointer to the head of the list.
 * \param node The node to insert.
 * \return The new head of the list.
 * \ingroup ill
**/
t_ill *ill_push(t_ill **head, t_ill *node) {
	if (!head) {
		LOG(C_MINI__ILL_LOG_EARG);
		return (NULL);
	}
	node->next = *head;
	*head      = node;
	LOG(C_MINI__ILL_LOG_SUCCESS);
	return (*head);
}
/**
 * \brief Inserts an entire list of nodes at the front of the list.
 * \param head Pointer to the head of the list.
 * \param nodes The head of the list of nodes to insert.
 * \return The new head of the list.
 * \ingroup ill
**/
t_ill *ill_npush(t_ill **head, t_ill *nodes) {
	if (!head) {
		LOG(C_MINI__ILL_LOG_EARG);
		return (NULL);
	}
	ill_get_tail(nodes)->next = *head;
	*head                     = nodes;
	LOG(C_MINI__ILL_LOG_SUCCESS);
	return (*head);
}
/**
 * \brief Inserts a node at the end of the list.
 * \param head Pointer to the head of the list.
 * \param node The node to insert.
 * \return The head of the list.
 * \ingroup ill
**/
t_ill *ill_push_back(t_ill **head, t_ill *node) {
	if (!head) {
		LOG(C_MINI__ILL_LOG_EARG);
		return (NULL);
	}
	if (!*head) {
		ill_push(head, node);
		LOG(C_MINI__ILL_LOG_SUCCESS);
		return (*head);
	}
	ill_get_tail(*head)->next = node;
	node->next                = NULL;
	LOG(C_MINI__ILL_LOG_SUCCESS);
	return (*head);
}
