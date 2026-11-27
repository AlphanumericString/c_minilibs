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

/**
 * \brief Iterates over the entire list and applies the callback with position
 * and last-element information.
 * \param head The head of the list.
 * \param func The callback function to apply.
 * \param u_data User data to pass to the callback.
 * \return The user data pointer, even when the callback fails.
 * \ingroup ill
**/
void *ill_apply_it(const t_cm_ill *head, t_cm_callback_it func, void *u_data) {
	return (ill_napply_it(head, (size_t)-1, func, u_data));
}

/**
 * \brief Iterates over at most n elements and applies the callback with
 * position and last-element information.
 * \param head The head of the list.
 * \param n Maximum number of elements to process. (size_t)-1 means all.
 * \param func The callback function to apply.
 * \param u_data User data to pass to the callback.
 * \return The user data pointer, even when the callback fails.
 * \note last is true for the final element of the processed range, including
 * when n exceeds the list length.
 * \ingroup ill
**/
void *ill_napply_it(const t_cm_ill *head, size_t n, t_cm_callback_it func,
                    void *u_data) {
	const t_cm_ill *curr;
	size_t          pos = 0;

	if (!func || !head) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (u_data);
	}
	curr = head;
	while (curr && n) {
		const t_cm_ill *nxt = curr->next;
		if (!func(ill_ntoe(curr), u_data, pos, !nxt || n == 1)) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__ILL_LOG_ECB)
			return (u_data);
		}
		curr = nxt;
		n--;
		pos++;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (u_data);
}

/**
 * \brief Applies the callback to the elements of a bounded range.
 * \param head_elem The first element of the range.
 * \param end_elem The element ending the range (exclusive). NULL means apply up
 * to and including the tail.
 * \param c The callback function to apply.
 * \param u_data User data to pass to the callback.
 * \param offset Offset of the node in the range boundary elements.
 * \return The user data pointer, even when the callback fails.
 * \note stops early if the callback returns false.
 * \ingroup ill
**/
void *ill_apply_range(const void *head_elem, const void *end_elem,
                      t_cm_callback c, void *u_data, size_t offset) {
	const t_cm_ill *curr;
	const t_cm_ill *end;

	if (!head_elem || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (u_data);
	}
	curr = ill_eton(head_elem, offset);
	end  = end_elem ? ill_eton(end_elem, offset) : NULL;
	while (curr && curr != end) {
		const t_cm_ill *nxt = curr->next;
		if (!c(ill_ntoe(curr), u_data)) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__ILL_LOG_ECB)
			return (u_data);
		}
		curr = nxt;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (u_data);
}
