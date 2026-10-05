#include "intrusive_doubly_linked.h"

/**
 * \brief Retrieves the tail (last node) of the list.
 * \param node A node of the list.
 * \return A pointer to the last node in the list, or NULL if the list is empty.
 * \ingroup idl
**/
t_idl *idl_get_tail(t_idl *node) {
	if (!node) {
		LOG(C_MINI__IDL_LOG_EARG);
		return NULL;
	}
	for (; node->next; node = node->next)
		;
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return node;
}
/**
 * \brief Retrieves the head (first node) of the list.
 * \param node A node of the list.
 * \return A pointer to the last node in the list, or NULL if the list is empty.
 * \ingroup idl
**/
t_idl *idl_get_head(t_idl *node) {

	if (!node) {
		LOG(C_MINI__IDL_LOG_EARG);
		return NULL;
	}
	for (; node->prev; node = node->prev)
		;
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return (node);
}
// TODO: idl_advance_it + idl_recede_it (return node->[next|prev]);
// TODO: ill_get_head + ill_[advance|recede]_it
// TODO: tests
