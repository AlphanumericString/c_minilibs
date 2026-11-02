#include "intrusive_linked_lists.h"

/**
 * \brief Advances an iterator one node forward and returns the next element.
 * \param e Pointer to the current element (user structure).
 * \param offset Offset of the intrusive node within the element.
 * \return A pointer to the next element, or NULL if there is none.
 * \ingroup ill
**/
void *ill_advance_it(void *e, size_t offset) {
	const t_ill *node;

	if (!e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	node = ill_eton(e, offset);
	node = node->next;
	if (!node) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_FAIL)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return ill_ntoe(node);
}

/**
 * \brief Advances a node iterator one node forward.
 * \param it The current node.
 * \return The next node, or NULL if there is none.
 * \ingroup ill
**/
t_ill *ill_advance_it_node(const t_ill *it) {
	if (!it) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return it->next;
}
