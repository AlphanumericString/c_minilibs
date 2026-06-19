#include "intrusive_doubly_linked.h"

/**
 * \brief Removes and returns the head node of the list.
 * \param head Pointer to the head of the list.
 * \return The removed head node, or NULL if the list was empty.
**/
t_idl *idl_pop(t_idl **head) {
	t_idl *to_remove;

	if (!*head) return NULL;
	to_remove       = *head;
	*head           = (*head)->next;
	to_remove->next = NULL;
	return to_remove;
}
/**
 * \brief Removes and returns the tail node of the list.
 * \param head Pointer to the head of the list.
 * \return The removed tail node, or NULL if the list was empty.
**/
t_idl *idl_pop_back(t_idl **head) {
	t_idl *to_remove;
	t_idl *ret;

	if (!(*head)->next) return (idl_pop(head));
	to_remove = *head;
	while (to_remove->next->next)
		to_remove = to_remove->next;
	ret             = to_remove->next;
	to_remove->next = NULL;
	return (ret);
}
