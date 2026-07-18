#include "intrusive_doubly_linked.h"

/**
 * \brief Retrieves the tail (last node) of the list.
 * \param head The head of the list.
 * \return A pointer to the last node in the list, or NULL if the list is empty.
 * \ingroup idl
 **/
t_idl *idl_get_tail(t_idl *head) {
	if (!head) {
		LOG(C_MINI__IDL_LOG_EARG);
		return NULL;
	}
	while (head->next)
		head = head->next;
	LOG(C_MINI__IDL_LOG_SUCCESS);
	return (t_idl *)head;
}
