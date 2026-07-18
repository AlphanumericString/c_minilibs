#include "intrusive_linked_lists.h"

/**
 * \brief Initializes a node with the given offset.
 * \param node The node to initialize.
 * \param offset The offset of the node within its container structure.
 * \ingroup ill
 **/
void ill_init(t_ill *node, size_t offset) {
	if (!node) {
		LOG(C_MINI__ILL_LOG_EARG);
		return;
	}
	node->next   = NULL;
	node->offset = offset;
	LOG(C_MINI__ILL_LOG_SUCCESS);
}

static void setup_chain(t_ill *to_setup, t_ill *nxt, size_t offset) {
	to_setup->next   = nxt;
	to_setup->offset = offset;
}

/**
 * \brief Creates a deep copy of the list using the provided duplicator
 * function.
 * \param head The head of the list to copy.
 * \param dupper The duplicator function used to copy each element.
 * \return A pointer to the head of the new copied list's first element, or NULL
 * on failure.
 * \ingroup ill
 **/
void *ill_copy(const t_ill *head, t_duplicator dupper) {
	size_t offset;
	void  *e_curr;
	t_ill *prv;
	void  *new_head;

	if (!head) {
		LOG(C_MINI__ILL_LOG_EARG);
		return (NULL);
	}
	offset = head->offset;
	e_curr = dupper(ill_ntoe(head));
	if (!e_curr) {
		LOG(C_MINI__ILL_LOG_EALLOC);
		return (NULL);
	}
	new_head = e_curr;
	prv      = ill_eton(e_curr, offset);
	head     = head->next;
	while (head) {
		e_curr = dupper(ill_ntoe(head));
		if (!e_curr) {
			LOG(C_MINI__ILL_LOG_EALLOC);
			break; // TODO: maybe a clearlist here from new_head if fail on
			       // dupper?
		}
		setup_chain(prv, ill_eton(e_curr, offset), offset);
		prv  = prv->next;
		head = head->next;
	}
	setup_chain(prv, NULL, offset);
	LOG(C_MINI__ILL_LOG_SUCCESS);
	return (new_head);
}
