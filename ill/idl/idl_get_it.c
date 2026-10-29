#include "intrusive_doubly_linked.h"

/**
 * \brief Retrieves the tail (last node) of the list.
 * \param node A node of the list.
 * \return A pointer to the last node in the list, or NULL if the list is empty.
 * \ingroup idl
**/
t_idl *idl_get_tail(t_idl *node) {
	if (!node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return NULL;
	}
	for (; node->next; node = node->next)
		;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return node;
}
/**
 * \brief Retrieves the head (first node) of the list.
 * \param node A node of the list.
 * \return A pointer to the first node in the list, or NULL if the list is
 * empty.
 * \ingroup idl
**/
t_idl *idl_get_head(t_idl *node) {

	if (!node) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return NULL;
	}
	for (; node->prev; node = node->prev)
		;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (node);
}
