#include "intrusive_linked_lists.h"

/**
 * \brief Inserts a node immediately after an anchor node.
 * \param anchor The node after which to insert (must be in a list).
 * \param node The node to insert.
**/
void ill_append(t_ill *anchor, t_ill *node) {
	node->next   = anchor->next;
	anchor->next = node;
}
/**
 * \brief Appends an entire list of nodes after an anchor node.
 * \param anchor The node after which to insert.
 * \param nodes The head of the list of nodes to insert.
**/
void ill_nappend(t_ill *anchor, t_ill *nodes) {
	ill_get_tail(nodes)->next = anchor->next;
	anchor->next = nodes;
}
