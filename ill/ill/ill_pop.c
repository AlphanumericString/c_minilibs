#include "intrusive_linked_lists.h"

/**
 * \brief Removes and returns the head node of the list.
 * \param head Pointer to the head of the list.
 * \return The removed head node, or NULL if the list was empty.
 * \ingroup ill
**/
t_ill *ill_pop(t_ill **head) {
	t_ill *to_remove;

	if (!*head) {
		LOG(C_MINI__ILL_LOG_EARG);
		return NULL;
	}
	to_remove       = *head;
	*head           = (*head)->next;
	to_remove->next = NULL;
	LOG(C_MINI__ILL_LOG_SUCCESS);
	return to_remove;
}
/**
 * \brief Removes and returns the tail node of the list.
 * \param head Pointer to the head of the list.
 * \return The removed tail node, or NULL if the list was empty.
 * \ingroup ill
**/
t_ill *ill_pop_back(t_ill **head) {
	t_ill *to_remove;
	t_ill *ret;

	if (!*head) {
		LOG(C_MINI__ILL_LOG_EARG);
		return NULL;
	}
	if (!(*head)->next) {
		ret = ill_pop(head);
		LOG(C_MINI__ILL_LOG_SUCCESS);
		return (ret);
	}
	to_remove = *head;
	while (to_remove->next->next)
		to_remove = to_remove->next;
	ret             = to_remove->next;
	to_remove->next = NULL;
	LOG(C_MINI__ILL_LOG_SUCCESS);
	return (ret);
}
