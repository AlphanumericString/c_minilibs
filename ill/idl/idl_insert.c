#include "intrusive_doubly_linked.h"

/**
 * \brief Inserts a node immediately after an anchor node.
 * \param anchor The node after which to insert (must be in a list).
 * \param node The node to insert.
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

void idl_prepend(t_idl *anchor, t_idl *node) {
	if (!anchor || !node) {
		LOG(C_MINI__IDL_LOG_EARG);
		return;
	}
	t_idl *t = anchor->prev;

	node->next   = anchor;
	anchor->prev = node;
	node->prev   = t;
	if (t) t->next = node;
	LOG(C_MINI__IDL_LOG_SUCCESS);
}
void idl_nprepend(t_idl *anchor, t_idl *nodes) {
	t_idl *t;
	t_idl *ln;

	if (!anchor || !nodes) {
		LOG(C_MINI__IDL_LOG_EARG);
		return;
	}
	ln = idl_get_tail(nodes);
	t  = anchor->prev;
	ln->next     = anchor;
	anchor->prev = ln;
	nodes->prev  = t;
	if (t) t->next = nodes;
	LOG(C_MINI__IDL_LOG_SUCCESS);
}
