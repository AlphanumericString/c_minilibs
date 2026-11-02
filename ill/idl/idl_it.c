#include "intrusive_doubly_linked.h"

/**
 * \brief Advances an iterator one node forward and returns the next element.
 * \param e Pointer to the current element (user structure).
 * \param offset Offset of the intrusive node within the element.
 * \return A pointer to the next element, or NULL if there is none.
 * \ingroup idl
**/
void *idl_advance_it(const void *e, size_t offset) {
	const t_idl *node;

	if (!e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	node = idl_eton(e, offset);
	node = node->next;
	if (!node) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return idl_ntoe(node);
}

/**
 * \brief Recedes an iterator one node backward and returns the previous
 * element.
 * \param e Pointer to the current element (user structure).
 * \param offset Offset of the intrusive node within the element.
 * \return A pointer to the previous element, or NULL if there is none.
 * \ingroup idl
**/
void *idl_recede_it(const void *e, size_t offset) {
	const t_idl *node;

	if (!e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	node = idl_eton(e, offset);
	node = node->prev;
	if (!node) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_FAIL)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return idl_ntoe(node);
}

/**
 * \brief Advances a node iterator one node forward.
 * \param it The current node.
 * \return The next node, or NULL if there is none.
 * \ingroup idl
**/
t_idl *idl_advance_it_node(const t_idl *it) {
	if (!it) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return it->next;
}

/**
 * \brief Recedes a node iterator one node backward.
 * \param it The current node.
 * \return The previous node, or NULL if there is none.
 * \ingroup idl
**/
t_idl *idl_recede_it_node(const t_idl *it) {
	if (!it) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return it->prev;
}
