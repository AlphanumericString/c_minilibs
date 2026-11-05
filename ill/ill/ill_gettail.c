#include "intrusive_linked_lists.h"

/**
 * \brief Retrieves the tail (last node) of the list.
 * \param head The head of the list.
 * \return A pointer to the last node in the list, or NULL if the list is empty.
 * \ingroup ill
**/
t_cm_ill *ill_get_tail(t_cm_ill *head) {
	if (!head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return NULL;
	}
	while (head->next)
		head = head->next;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (t_cm_ill *)head;
}
