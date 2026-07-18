#include "intrusive_doubly_linked.h"

/**
 * \brief Inserts a node immediately after an anchor node.
 * \param anchor The node after which to insert (must be in a list).
 * \param node The node to insert.
 * \ingroup idl
 **/
void idl_append(t_idl *anchor, t_idl *node) {
	if (!anchor || !node) {
		LOG(C_MINI__IDL_LOG_EARG);
		return;
	}
	node->next = anchor->next;
	if (anchor->next) anchor->next->prev = node;
	node->prev   = anchor;
	anchor->next = node;
	LOG(C_MINI__IDL_LOG_SUCCESS);
}
/**
 * \brief Appends an entire list of nodes after an anchor node.
 * \param anchor The node after which to insert.
 * \param nodes The head of the list of nodes to insert.
 * \ingroup idl
 **/
void idl_nappend(t_idl *anchor, t_idl *nodes) {
	if (!anchor || !nodes) {
		LOG(C_MINI__IDL_LOG_EARG);
		return;
	}
	t_idl *tail = idl_get_tail(nodes);
	tail->next  = anchor->next;
	if (anchor->next) anchor->next->prev = tail;
	anchor->next = nodes;
	nodes->prev  = anchor;
	LOG(C_MINI__IDL_LOG_SUCCESS);
}

/**
 * \brief Inserts a node immediately before an anchor node.
 * \param anchor The node before which to insert.
 * \param node The node to insert.
 * \ingroup idl
 **/
void idl_prepend(t_idl *anchor, t_idl *node) {
	if (!anchor || !node) {
		LOG(C_MINI__IDL_LOG_EARG);
		return;
	}
	t_idl *anchor_prev = anchor->prev;
	node->next   = anchor;
	anchor->prev = node;
	node->prev   = anchor_prev;
	if (anchor_prev) anchor_prev->next = node;
	LOG(C_MINI__IDL_LOG_SUCCESS);
}
/**
 * \brief Prepend (append in front) an entire list of nodes before an anchor
 * node.
 * \param anchor The node before which to insert.
 * \param nodes The head of the list of nodes to insert.
 * \ingroup idl
 **/
void idl_nprepend(t_idl *anchor, t_idl *nodes) {
	t_idl *anchor_prev;
	t_idl *last_node;

	if (!anchor || !nodes) {
		LOG(C_MINI__IDL_LOG_EARG);
		return;
	}
	last_node       = idl_get_tail(nodes);
	anchor_prev     = anchor->prev;
	last_node->next = anchor;
	anchor->prev    = last_node;
	nodes->prev     = anchor_prev;
	if (anchor_prev) anchor_prev->next = nodes;
	LOG(C_MINI__IDL_LOG_SUCCESS);
}
