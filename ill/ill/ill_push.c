#include "intrusive_linked_lists.h"

/**
 * \brief Inserts a node at the front of the list.
 * \param head Pointer to the head of the list.
 * \param node The node to insert.
 * \return The new head of the list.
 * \ingroup ill
**/
t_cm_ill *ill_push(t_cm_ill **head, t_cm_ill *node) {
	if (!head || !node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	node->next = *head;
	*head      = node;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (*head);
}
/**
 * \brief Inserts an entire list of nodes at the front of the list.
 * \param head Pointer to the head of the list.
 * \param nodes The head of the list of nodes to insert.
 * \return The new head of the list.
 * \ingroup ill
**/
t_cm_ill *ill_npush(t_cm_ill **head, t_cm_ill *nodes) {
	if (!head || !nodes) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	ill_get_tail(nodes)->next = *head;
	*head                     = nodes;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (*head);
}
/**
 * \brief Inserts a node at the end of the list.
 * \param head Pointer to the head of the list.
 * \param node The node to insert.
 * \return The head of the list.
 * \ingroup ill
**/
t_cm_ill *ill_push_back(t_cm_ill **head, t_cm_ill *node) {
	if (!head || !node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	if (!*head) {
		ill_push(head, node);
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
		return (*head);
	}
	ill_get_tail(*head)->next = node;
	node->next                = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (*head);
}
