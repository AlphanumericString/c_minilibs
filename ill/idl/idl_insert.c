#include "intrusive_doubly_linked.h"

/**
 * \brief Inserts a node immediately after an anchor node.
 * \param anchor The node after which to insert (must be in a list).
 * \param node The node to insert.
 * \ingroup idl
**/
void idl_append(t_idl *anchor, t_idl *node) {
	if (!anchor || !node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return;
	}
	node->next = anchor->next;
	node->prev = anchor;
	if (anchor->next) anchor->next->prev = node;
	anchor->next = node;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
}
/**
 * \brief Appends an entire list of nodes after an anchor node.
 * \param anchor The node after which to insert.
 * \param nodes The head of the list of nodes to insert.
 * \ingroup idl
**/
void idl_nappend(t_idl *anchor, t_idl *nodes) {
	t_idl *first_node, *last_node;

	if (!anchor || !nodes) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return;
	}
	last_node  = idl_get_tail(nodes);
	first_node = idl_get_head(nodes);

	last_node->next  = anchor->next;
	first_node->prev = anchor;
	if (anchor->next) anchor->next->prev = last_node;
	anchor->next = first_node;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
}

/**
 * \brief Inserts a node immediately before an anchor node.
 * \param anchor The node before which to insert.
 * \param node The node to insert.
 * \ingroup idl
**/
void idl_prepend(t_idl *anchor, t_idl *node) {
	if (!anchor || !node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return;
	}
	node->prev = anchor->prev;
	if (anchor->prev) anchor->prev->next = node;
	node->next   = anchor;
	anchor->prev = node;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
}
/**
 * \brief Prepend (append in front) an entire list of nodes before an anchor
 * node.
 * \param anchor The node before which to insert.
 * \param nodes The head of the list of nodes to insert.
 * \ingroup idl
**/
void idl_nprepend(t_idl *anchor, t_idl *nodes) {
	t_idl *last_node, *first_node;

	if (!anchor || !nodes) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return;
	}
	last_node  = idl_get_tail(nodes);
	first_node = idl_get_head(nodes);

	first_node->prev = anchor->prev;
	last_node->next  = anchor;
	if (anchor->prev) anchor->prev->next = first_node;
	anchor->prev = last_node;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
}
