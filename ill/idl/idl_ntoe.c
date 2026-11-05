#include "intrusive_doubly_linked.h"

/**
 * \brief Gets the pointer to the element containing the node.
 * \param node The intrusive node.
 * \return A pointer to the user's element structure.
 * \ingroup idl
**/
void *idl_ntoe(const t_cm_idl *node) {
	if (!node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return ((t_cm_idl *)NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return ((void *)(((const char *)node) - node->offset));
}
/**
 * \brief Gets the intrusive node within an element structure.
 * \param e Pointer to the user's element structure.
 * \param offset The offset of the node within the structure.
 * \return A pointer to the intrusive node.
 * \ingroup idl
**/
t_cm_idl *idl_eton(const void *e, size_t offset) {
	if (!e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return ((t_cm_idl *)NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return ((t_cm_idl *)((char *)e + offset));
}
