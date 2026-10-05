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

static inline void setup_chain(t_ill *to_setup, t_ill *nxt, size_t offset) {
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
	void  *e_cpy = NULL;
	t_ill *prev  = NULL;
	void  *nh    = NULL;

	if (!head || !dupper) {
		LOG(C_MINI__ILL_LOG_EARG);
		return (NULL);
	}
	offset = head->offset;
	while (head) {
		e_cpy = dupper(ill_ntoe(head));
		if (!e_cpy) {
			LOG(C_MINI__ILL_LOG_EALLOC);
			// TODO: maybe clearlist here from new_head if fail on dupper?
			break;
		}
		if (!prev) {
			nh   = e_cpy;
			prev = ill_eton(e_cpy, offset);
		} else {
			setup_chain(prev, ill_eton(e_cpy, offset), offset);
			prev = prev->next;
		}
		head = head->next;
	}
	setup_chain(prev, NULL, offset);
	LOG(C_MINI__ILL_LOG_SUCCESS);
	return (nh);
}
