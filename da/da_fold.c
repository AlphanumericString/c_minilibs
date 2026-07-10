#include <stdlib.h>

#include "dynamic_array.h"

/**
 * \brief fold the elements of the dynamic array into a single value
 * \param da pointer to the dynamic array
 * \param m merger function
 * \param u_start_val starting value
 * \return the folded value
**/
void *da_fold(t_da *da, t_merger m, void *u_start_val) {
	return (da_nfold(da, -1, m, u_start_val));
}

/**
 * \brief fold the first n elements of the dynamic array into a single value
 * \param da pointer to the dynamic array
 * \param n number of elements to fold
 * \param m merger function
 * \param u_start_val starting value
 * \return the folded value
**/
void *da_nfold(t_da *da, size_t n, t_merger m, void *u_start_val) {
	if (!m || !u_start_val || !da) return (NULL);
	if (n > da->nb_e) n = da->nb_e;
	for (size_t i = 0; i < n; i++)
		u_start_val = m(da_at(da, i), u_start_val);
	return (u_start_val);
}

/**
 * \brief reduce the elements of the dynamic array into a newly allocated value
 * \param da pointer to the dynamic array
 * \param m merger function
 * \return the reduced value
**/
void *da_reduce(t_da *da, t_merger m) {
	return (da_nreduce(da, -1, m));
}

/**
 * \brief reduce the first n elements of the dynamic array into a newly
 * allocated value
 * \param da pointer to the dynamic array
 * \param n number of elements to reduce
 * \param m merger function
 * \return the reduced value
**/
void *da_nreduce(t_da *da, size_t n, t_merger m) {
	void *bag;

	if (!m || !da) return (NULL);
	if (n > da->nb_e) n = da->nb_e;
	bag = calloc(1, da->sz_e);
	if (!bag) return (bag);
	for (size_t i = 0; i < n; i++)
		bag = m(da_at(da, i), bag);
	return (bag);
}
