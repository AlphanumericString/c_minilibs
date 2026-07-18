#include "intrusive_doubly_linked.h"

/**
 * \brief Initializes a node with the given offset.
 * \param node The node to initialize.
 * \param offset The offset of the node within its container structure.
 * \ingroup idl
**/
void idl_init(t_idl *node, size_t offset) {
	if (!node) {
		LOG(C_MINI__IDL_LOG_EARG);
		return;
	}
	node->next   = NULL;
	node->prev   = NULL;
	node->offset = offset;
	LOG(C_MINI__IDL_LOG_SUCCESS);
}

/**
 * \brief Creates a deep copy of the list using the provided duplicator
 * function.
 * \param head The head of the list to copy.
 * \param d The duplicator function used to copy each element.
 * \return A pointer to the head of the new copied list's first element, or NULL
 * on failure.
 * \ingroup idl
**/
void *idl_copy(const t_idl *head, t_duplicator d) {
	t_idl *ret;
	void  *elem;
	void  *nh = NULL;

	if (!head) {
		LOG(C_MINI__IDL_LOG_EARG);
		return (NULL);
	}
	ret = NULL;
	for (; head; head = head->next) {
		elem = d(idl_ntoe(head));
		if (!elem) return (nh); // TODO: mayb delete chain if fail
		if (!nh) nh = elem;
		if (ret) ret->next = idl_eton(elem, head->offset);
		idl_eton(elem, head->offset)->offset = head->offset;
		idl_eton(elem, head->offset)->prev   = ret;
		idl_eton(elem, head->offset)->next   = NULL;
		ret                                  = idl_eton(elem, head->offset);
	}
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return (nh);
}
