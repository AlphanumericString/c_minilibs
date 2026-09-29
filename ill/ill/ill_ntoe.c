#include "intrusive_linked_lists.h"

/**
 * \brief Gets the pointer to the element containing the node.
 * \param node The intrusive node.
 * \return A pointer to the user's element structure.
 * \ingroup ill
**/
void *ill_ntoe(const t_ill *node) {
	if (!node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS);
	return ((void *)(((char *)node) - node->offset));
}
/**
 * \brief Gets the intrusive node within an element structure.
 * \param e Pointer to the user's element structure.
 * \param offset The offset of the node within the structure.
 * \return A pointer to the intrusive node.
 * \ingroup ill
**/
t_ill *ill_eton(void *e, size_t offset) {
	if (!e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS);
	return ((t_ill *)((char *)e + offset));
}
