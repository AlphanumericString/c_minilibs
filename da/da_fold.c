#include <stdlib.h>

#include "dynamic_array.h"

/**
 * \brief fold the elements of the dynamic array into a single value
 * \param da pointer to the dynamic array
 * \param m merger function
 * \param data starting value
 * \return the folded value
 * \ingroup da
**/
void *da_fold(t_da *da, t_merger m, void *data) {
	if (!da_is_valid(da) || !m || !data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG);
		return (NULL);
	}
	return (da_nfold(da, -1, m, data));
}
/**
 * \brief fold the first n elements of the dynamic array into a single value
 * \param da pointer to the dynamic array
 * \param n number of elements to fold
 * \param m merger function
 * \param data starting value
 * \return the folded value
 * \ingroup da
**/
void *da_nfold(t_da *da, size_t n, t_merger m, void *data) {
	if (!da_is_valid(da) || !m || !data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG);
		return (NULL);
	}
	if (n > da->nb_e) n = da->nb_e;
	for (size_t i = 0; i < n; i++)
		data = m(da_at(da, i), data);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS);
	return (data);
}

/**
 * \brief reduce the elements of the dynamic array into a newly allocated value
 * \param da pointer to the dynamic array
 * \param m merger function
 * \return the reduced value or null if any error
 * \ingroup da
**/
void *da_reduce(t_da *da, t_merger m) {
	void *r;

	if (!da_is_valid(da) || !m) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG);
		return (NULL);
	}
	r = da_nreduce(da, -1, m);
	if (!r) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS);
	return (r);
}
/**
 * \brief reduce the first n elements of the dynamic array into a newly
 * allocated value
 * \param da pointer to the dynamic array
 * \param n number of elements to reduce
 * \param m merger function
 * \return the reduced valu or null if any errore
 * \ingroup da
**/
void *da_nreduce(t_da *da, size_t n, t_merger m) {
	void *bag;

	if (!da_is_valid(da) || !m) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG);
		return (NULL);
	}
	if (n > da->nb_e) n = da->nb_e;
	bag = calloc(1, da->sz_e);
	if (!bag) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC);
		return (NULL);
	}
	for (size_t i = 0; i < n; i++)
		bag = m(da_at(da, i), bag);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS);
	return (bag);
}
