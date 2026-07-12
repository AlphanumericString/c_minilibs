#include "intrusive_linked_lists.h"

/**
 * \brief Iterates over the entire list and applies the callback to each
 * element.
 * \param head The head of the list.
 * \param c The callback function to apply.
 * \param u_data User data to pass to the callback.
 * \return true if the callback returned true for all elements, false otherwise.
 **/
bool ill_foreach(t_ill *head, t_callback c, void *u_data) {
	t_ill *curr;
	t_ill *nxt;

	if (!c || !head) {
		LOG(C_MINI__ILL_LOG_EARG);
		return (false);
	}
	curr = head;
	while (curr) {
		nxt = curr->next;
		if (!c(ill_ntoe(curr), u_data)) {
			LOG(C_MINI__ILL_LOG_ECB);
			return (false);
		}
		curr = nxt;
	}
	LOG(C_MINI__ILL_LOG_SUCCESS);
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
 **/
bool ill_for(t_ill *head, size_t n, t_callback c, void *u_data) {
	t_ill *curr;
	t_ill *nxt;

	if (!c || !head) {
		LOG(C_MINI__ILL_LOG_EARG);
		return (false);
	}
	curr = head;
	while (curr && n) {
		nxt = curr->next;
		if (!c(ill_ntoe(curr), u_data)) {
			LOG(C_MINI__ILL_LOG_ECB);
			return (false);
		}
		curr = nxt;
		n--;
	}
	LOG(C_MINI__ILL_LOG_SUCCESS);
	return (true);
}
