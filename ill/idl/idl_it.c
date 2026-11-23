#include "intrusive_doubly_linked.h"

/**
 * \brief Advances an iterator one node forward and returns the next element.
 * \param e Pointer to the current element (user structure).
 * \param offset Offset of the intrusive node within the element.
 * \return A pointer to the next element, or NULL if there is none.
 * \note Stateless equivalent of "it++". See idl_iter() for the pull iterator.
 * \ingroup idl
**/
void *idl_advance_it(const void *e, size_t offset) {
	const t_cm_idl *node;

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
 * \note Stateless equivalent of "it--". See idl_advance_it().
 * \ingroup idl
**/
void *idl_recede_it(const void *e, size_t offset) {
	const t_cm_idl *node;

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
t_cm_idl *idl_advance_it_node(const t_cm_idl *it) {
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
t_cm_idl *idl_recede_it_node(const t_cm_idl *it) {
	if (!it) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return it->prev;
}

/**
 * \brief Pull iterator over the elements of the list.
 * \param head The head of the list.
 * \param iter Element pointer used as the iteration state. It must be
 * initialized to NULL before the first call; on a true return it holds the
 * current element.
 * \param offset Offset of the node in the current element. Pass the offset of
 * the element currently stored in *iter; mixed-offset lists are supported as
 * long as the caller tracks the current element type. The offset is ignored on
 * the first call.
 * \return true while an element is available, false once the list is exhausted
 * or on invalid arguments.
 * \note typical usage:
 * \code
 * void *it = NULL;
 * while (idl_iter(head, &it, offsetof(t_elem, node)))
 *     do_something(it);
 * \endcode
 * \note idl_iter() keeps the current element in *iter; idl_advance_it() is the
 * stateless "it++" equivalent.
 * \ingroup idl
**/
bool idl_iter(const t_cm_idl *head, void **iter, size_t offset) {
	const t_cm_idl *node;

	if (!head || !iter) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		if (iter) *iter = NULL;
		return (false);
	}
	if (!*iter) {
		*iter = idl_ntoe(head);
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (true);
	}
	node = idl_eton(*iter, offset);
	node = node->next;
	if (!node) {
		*iter = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return (false);
	}
	*iter = idl_ntoe(node);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (true);
}
