#include "intrusive_doubly_linked.h"

/**
 * \brief Initializes a node with the given offset.
 * \param node The node to initialize.
 * \param offset The offset of the node within its container structure.
 * \ingroup idl
**/
void idl_init(t_cm_idl *node, size_t offset) {
	if (!node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return;
	}
	node->next   = NULL;
	node->prev   = NULL;
	node->offset = offset;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
}

static void clean_chain(const void *nh, size_t offset, t_cm_dtor dtor) {
	t_cm_idl *curr;

	if (!nh || !dtor) return;
	curr = idl_eton(nh, offset);
	while (curr) {
		t_cm_idl *nxt = curr->next;
		dtor(idl_ntoe(curr));
		curr = nxt;
	}
}

/**
 * \brief Creates a deep copy of the list using the provided duplicator
 * function.
 * \param head The head of the list to copy.
 * \param d The duplicator function used to copy each element.
 * \param dtor Optional destructor used to clean-up already copied elements if a
 * later duplication fails.
 * \return A pointer to the head of the new copied list's first element, or NULL
 * on failure.
 * \ingroup idl
**/
void *idl_copy(const t_cm_idl *head, t_cm_duplicator d, t_cm_dtor dtor) {
	size_t    offset;
	t_cm_idl *ret;
	void     *elem;
	void     *nh = NULL;

	if (!head || !d) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	offset = head->offset;
	ret    = NULL;
	for (; head; head = head->next) {
		elem = d(idl_ntoe(head));
		if (!elem) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__IDL_LOG_EALLOC)
			clean_chain(nh, offset, dtor);
			return (NULL);
		}
		if (!nh) nh = elem;
		if (ret) ret->next = idl_eton(elem, offset);
		idl_eton(elem, offset)->offset = offset;
		idl_eton(elem, offset)->prev   = ret;
		idl_eton(elem, offset)->next   = NULL;
		ret                            = idl_eton(elem, offset);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (nh);
}
