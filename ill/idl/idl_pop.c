#include "intrusive_doubly_linked.h"

/**
 * \brief Removes and returns the head node of the list.
 * \param head Pointer to the head of the list.
 * \return The removed head node, or NULL if the list was empty.
 * \ingroup idl
**/
t_idl *idl_pop(t_idl **head) {
	t_idl *to_remove;

	if (!head || !*head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return NULL;
	}
	to_remove = *head;
	*head     = (*head)->next;
	if (*head) (*head)->prev = NULL;
	to_remove->next = NULL;
	to_remove->prev = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return to_remove;
}
/**
 * \brief Removes and returns the tail node of the list.
 * \param head Pointer to the head of the list.
 * \return The removed tail node, or NULL if the list was empty.
 * \ingroup idl
**/
t_idl *idl_pop_back(t_idl **head) {
	t_idl *ret;

	if (!head || !*head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return NULL;
	}
	if (!(*head)->next) {
		ret = idl_pop(head);
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
		return ret;
	}
	ret = idl_get_tail(*head);
	if (ret->prev) ret->prev->next = NULL;
	ret->next = NULL;
	ret->prev = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return ret;
}
