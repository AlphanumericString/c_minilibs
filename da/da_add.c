#include "./dynamic_array.h"

/**
 * \brief add an element, if needed grow the da
 * \param vec pointer to the dynamic array
 * \param elem pointer to the element to add
 * \return true if success, false if failure
 */
bool da_add(t_da *vec, void *elem) {
	void *tmp;

	if (!vec || !elem || !vec->array || !vec->cap_e) return (false);
	if ((vec->nb_e + 1) > vec->cap_e) {
		tmp = malloc(vec->sz_e * (vec->cap_e * 1.5));
		memcpy(tmp, vec->array, vec->nb_e * vec->sz_e);
		free(vec->array);
		vec->array  = tmp;
		vec->cap_e *= 1.5;
	}
	memcpy(da_at(vec, vec->nb_e), elem, vec->sz_e);
	vec->nb_e++;
	return (true);
}

/**
 * \brief add nb_e elements of size sz_e from the array arr, if needed grows the da
 * \param vec pointer to the dynamic array
 * \param arr pointer to the array to concatenate
 * \param nb_e number of elements to add
 * \param sz_e size of the elements
 * \return true if success, false if failure
 */
bool da_cat_array(t_da *vec, void *arr, size_t nb_e, size_t sz_e) {
	void  *tmp;
	size_t new_cap;

	if (!arr || !vec || !sz_e || !vec->array) return (false);
	if (sz_e != vec->sz_e) return (false);
	new_cap = vec->cap_e;
	if (vec->nb_e + nb_e > vec->cap_e) {
		while ((vec->nb_e + nb_e) > new_cap)
			new_cap *= 1.5;
		tmp = malloc(sz_e * new_cap);
		memcpy(tmp, vec->array, vec->nb_e * vec->sz_e);
		free(vec->array);
		vec->array = tmp;
		vec->cap_e = new_cap;
	}
	memcpy(da_at(vec, vec->nb_e), arr, vec->sz_e * nb_e);
	vec->nb_e += nb_e;
	return (true);
}