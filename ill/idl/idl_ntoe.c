#include "intrusive_doubly_linked.h"

/**
 * \brief Gets the pointer to the element containing the node.
 * \param node The intrusive node.
 * \return A pointer to the user's element structure.
 **/
void *idl_ntoe(const t_idl *node) {
	if (!node) {
		LOG(C_MINI__IDL_LOG_EARG);
		return (NULL);
	}
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return ((void *)(((char *)node) - node->offset));
}
/**
 * \brief Gets the intrusive node within an element structure.
 * \param e Pointer to the user's element structure.
 * \param offset The offset of the node within the structure.
 * \return A pointer to the intrusive node.
 **/
t_idl *idl_eton(void *e, size_t offset) {
	if (!e) {
		LOG(C_MINI__IDL_LOG_EARG);
		return (NULL);
	}
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return ((t_idl *)((char *)e + offset));
}
