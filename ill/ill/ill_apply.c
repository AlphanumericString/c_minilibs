#include "intrusive_linked_lists.h"

/**
 * \brief Iterates over the entire list and applies the callback to each
 * element.
 * \param head The head of the list.
 * \param c The callback function to apply.
 * \param u_data User data to pass to the callback.
 * \return The user data pointer, even when the callback fails.
 * \ingroup ill
**/
void *ill_apply(const t_cm_ill *head, t_cm_callback c, void *u_data) {
	return (ill_napply(head, (size_t)-1, c, u_data));
}

/**
 * \brief Iterates over at most n elements of the list and applies the callback.
 * \param head The head of the list.
 * \param n Maximum number of elements to process. (size_t)-1 means all.
 * \param c The callback function to apply.
 * \param u_data User data to pass to the callback.
 * \return The user data pointer, even when the callback fails.
 * \ingroup ill
**/
void *ill_napply(const t_cm_ill *head, size_t n, t_cm_callback c,
                 void *u_data) {
	const t_cm_ill *curr;
	const t_cm_ill *nxt;

	if (!c || !head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (u_data);
	}
	curr = head;
	while (curr && n) {
		nxt = curr->next;
		if (!c(ill_ntoe(curr), u_data)) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__ILL_LOG_ECB)
			return (u_data);
		}
		curr = nxt;
		n--;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (u_data);
}
