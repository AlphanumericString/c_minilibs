#include "ill.h"

/**
 * \brief Initializes a node with the given offset.
 * \param node The node to initialize.
 * \param offset The offset of the node within its container structure.
**/
void ill_init(t_ill *node, size_t offset) {
	node->next   = NULL;
	node->offset = offset;
}

static void setup_chain(t_ill *to_setup, t_ill *nxt, size_t offset) {
	to_setup->next   = nxt;
	to_setup->offset = offset;
}

/**
 * \brief Creates a deep copy of the list using the provided duplicator
 * function.
 * \param head The head of the list to copy.
 * \param d The duplicator function used to copy each element.
 * \return A pointer to the head of the new copied list's first element, or NULL
 * on failure.
**/
void *ill_copy(const t_ill *head, t_duplicator d) {
	size_t offset;
	void  *e_curr;
	t_ill *prv;
	void  *nh;

	if (!head) return (NULL);
	offset = head->offset;
	e_curr = d(ill_ntoe(head));
	nh     = e_curr;
	if (!e_curr) return (NULL);
	prv = ill_eton(e_curr, offset);
	while (head) {
		e_curr = d(ill_ntoe(head));
		if (!e_curr) break;
		setup_chain(prv, ill_eton(e_curr, offset), offset);
		prv  = prv->next;
		head = head->next;
	}
	setup_chain(prv, NULL, offset);
	return (nh);
}
