#include "intrusive_doubly_linked.h"

/**
 * \brief Inserts a node immediately after an anchor node.
 * \param anchor The node after which to insert (must be in a list).
 * \param node The node to insert.
 **/
void idl_append(t_idl *anchor, t_idl *node) {
	node->next   = anchor->next;
	anchor->next = node;
}
/**
 * \brief Appends an entire list of nodes after an anchor node.
 * \param anchor The node after which to insert.
 * \param nodes The head of the list of nodes to insert.
 **/
void idl_nappend(t_idl *anchor, t_idl *nodes) {
	idl_get_tail(nodes)->next = anchor->next;
	anchor->next              = nodes;
}

void idl_prepend(t_idl *anchor, t_idl *node) {
	if (!anchor) return;
	t_idl *t;

	t            = anchor->prev;
	node->next   = anchor;
	anchor->prev = node;
	node->prev   = t;
}
void idl_nprepend(t_idl *anchor, t_idl *nodes) {
	t_idl *t;
	t_idl *ln;

	if (!anchor) return;
	ln = idl_get_tail(nodes);
	t  = anchor->prev;

	ln->next     = anchor;
	anchor->prev = ln;
	nodes->prev  = t;
}
