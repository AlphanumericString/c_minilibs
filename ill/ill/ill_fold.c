#include <stdlib.h>

#include "intrusive_linked_lists.h"

/**
 * \brief Folds the elements of the list into a single value.
 * \param head The head of the list.
 * \param m The merger function.
 * \param u_start The starting accumulator value.
 * \return The folded value, or NULL on invalid arguments.
 * \note an empty list returns u_start unchanged.
 * \ingroup ill
**/
void *ill_fold(const t_cm_ill *head, t_cm_merger m, void *u_start) {
	return (ill_nfold(head, (size_t)-1, m, u_start));
}

/**
 * \brief Folds at most n elements of the list into a single value.
 * \param head The head of the list.
 * \param n Maximum number of elements to fold. (size_t)-1 means all.
 * \param m The merger function.
 * \param u_start The starting accumulator value.
 * \return The folded value, or NULL on invalid arguments.
 * \note an empty list or n == 0 returns u_start unchanged.
 * \ingroup ill
**/
void *ill_nfold(const t_cm_ill *head, size_t n, t_cm_merger m, void *u_start) {
	if (!m || !u_start) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	while (head && n) {
		u_start = m(ill_ntoe(head), u_start);
		head    = head->next;
		n--;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__ILL_LOG_SUCCESS)
	return (u_start);
}

/**
 * \brief Reduces the elements of the list into a newly allocated value.
 * \param head The head of the list.
 * \param m The merger function.
 * \param elem_size Size of the accumulator to allocate.
 * \return The reduced value, or NULL on invalid arguments or allocation
 * failure.
 * \note the returned value is zero initialized and must be freed by the caller.
 * \ingroup ill
**/
void *ill_reduce(const t_cm_ill *head, t_cm_merger m, size_t elem_size) {
	return (ill_nreduce(head, (size_t)-1, m, elem_size));
}

/**
 * \brief Reduces at most n elements of the list into a newly allocated value.
 * \param head The head of the list.
 * \param n Maximum number of elements to reduce. (size_t)-1 means all.
 * \param m The merger function.
 * \param elem_size Size of the accumulator to allocate.
 * \return The reduced value, or NULL on invalid arguments or allocation
 * failure.
 * \note the returned value is zero initialized and must be freed by the caller.
 * \ingroup ill
**/
void *ill_nreduce(const t_cm_ill *head, size_t n, t_cm_merger m,
                  size_t elem_size) {
	void *acc;

	if (!m || !elem_size) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__ILL_LOG_EARG)
		return (NULL);
	}
	acc = calloc(1, elem_size);
	if (!acc) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__ILL_LOG_EALLOC)
		return (NULL);
	}
	return (ill_nfold(head, n, m, acc));
}
