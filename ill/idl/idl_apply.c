#include "intrusive_doubly_linked.h"

/**
 * \brief Iterates over the entire list and applies the callback to each
 * element.
 * \param head The head of the list.
 * \param c The callback function to apply.
 * \param u_data User data to pass to the callback.
 * \return true if the callback returned true for all elements, false otherwise.
 * \ingroup idl
**/
bool idl_apply(const t_cm_idl *head, t_cm_callback c, void *u_data) {

	if (!c || !head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (false);
	}
	if (idl_napply(head, -1, c, u_data) == false) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__IDL_LOG_ECB)
		return (false);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (true);
}

/**
 * \brief Iterates over at most n elements of the list and applies the callback.
 * \param head The head of the list.
 * \param n Maximum number of elements to process.
 * \param c The callback function to apply.
 * \param u_data User data to pass to the callback.
 * \return true if the callback returned true for all processed elements, false
 * otherwise.
 * \ingroup idl
**/
bool idl_napply(const t_cm_idl *head, size_t n, t_cm_callback c, void *u_data) {
	const t_cm_idl *curr;
	const t_cm_idl *nxt;

	if (!c || !head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__IDL_LOG_EARG)
		return (false);
	}
	curr = head;
	while (curr && n) {
		nxt = curr->next;
		if (!c(idl_ntoe(curr), u_data)) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__IDL_LOG_ECB)
			return (false);
		}
		curr = nxt;
		n--;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__IDL_LOG_SUCCESS)
	return (true);
}
