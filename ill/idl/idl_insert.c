#include "intrusive_doubly_linked.h"

/**
 * \brief Inserts a node immediately after an anchor node.
 * \param anchor The node after which to insert (must be in a list).
 * \param node The node to insert.
 * \ingroup idl
**/
void idl_append_node(t_cm_idl *anchor, t_cm_idl *node) {
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
 * \note nodes must be the head of the chain; the function does not walk back to
 * find it.
 * \ingroup idl
**/
void idl_nappend_node(t_cm_idl *anchor, t_cm_idl *nodes) {
	t_cm_idl *last_node;

	if (!anchor || !nodes) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return;
	}
	last_node = idl_get_tail_node(nodes);

	last_node->next = anchor->next;
	nodes->prev     = anchor;
	if (anchor->next) anchor->next->prev = last_node;
	anchor->next = nodes;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
}

/**
 * \brief Inserts a node immediately before an anchor node.
 * \param anchor The node before which to insert.
 * \param node The node to insert.
 * \ingroup idl
**/
void idl_prepend_node(t_cm_idl *anchor, t_cm_idl *node) {
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
 * \note nodes must be the head of the chain; the function does not walk back to
 * find it.
 * \ingroup idl
**/
void idl_nprepend_node(t_cm_idl *anchor, t_cm_idl *nodes) {
	t_cm_idl *last_node;

	if (!anchor || !nodes) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return;
	}
	last_node = idl_get_tail_node(nodes);

	nodes->prev     = anchor->prev;
	last_node->next = anchor;
	if (anchor->prev) anchor->prev->next = nodes;
	anchor->prev = last_node;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
}
