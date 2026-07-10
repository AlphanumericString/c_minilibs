#include "intrusive_doubly_linked.h"

/**
 * \brief Removes and returns the head node of the list.
 * \param head Pointer to the head of the list.
 * \return The removed head node, or NULL if the list was empty.
**/
t_idl *idl_pop(t_idl **head) {
	t_idl *to_remove;

	if (!*head) return NULL;
	to_remove = *head;
	*head     = (*head)->next;
	if (*head) (*head)->prev = NULL;
	to_remove->next = NULL;
	to_remove->prev = NULL;
	return to_remove;
}
/**
 * \brief Removes and returns the tail node of the list.
 * \param head Pointer to the head of the list.
 * \return The removed tail node, or NULL if the list was empty.
**/
t_idl *idl_pop_back(t_idl **head) {
	t_idl *ret;

	if (!*head) return NULL;
	if (!(*head)->next) return idl_pop(head);

	ret = idl_get_tail(*head);
	if (ret->prev) ret->prev->next = NULL;
	ret->next = NULL;
	ret->prev = NULL;
	return ret;
}
