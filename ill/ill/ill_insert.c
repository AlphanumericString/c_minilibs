#include "intrusive_linked_lists.h"

/**
 * \brief Inserts a node immediately after an anchor node.
 * \param anchor The node after which to insert (must be in a list).
 * \param node The node to insert.
 * \ingroup ill
**/
void ill_append(t_ill *anchor, t_ill *node) {
	if (!anchor || !node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG);
		return;
	}
	node->next   = anchor->next;
	anchor->next = node;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS);
}
/**
 * \brief Appends an entire list of nodes after an anchor node.
 * \param anchor The node after which to insert.
 * \param nodes The head of the list of nodes to insert.
 * \ingroup ill
**/
void ill_nappend(t_ill *anchor, t_ill *nodes) {
	if (!anchor || !nodes) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG);
		return;
	}
	ill_get_tail(nodes)->next = anchor->next;
	anchor->next              = nodes;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS);
}
