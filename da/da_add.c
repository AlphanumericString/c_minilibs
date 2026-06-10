#include "dynamic_array.h"

/**
 * \brief add an element, if needed grow the da
 * \param da pointer to the dynamic array
 * \param elem pointer to the element to add
 * \return true if success, false if failure
 **/
bool da_add(t_da *da, void *elem) {
	if (!da || !elem || !da->array || !da->cap_e) return (false);
	if ((da->nb_e + 1) > da->cap_e) da_grow(da);
	memcpy(da_at(da, da->nb_e), elem, da->sz_e);
	da->nb_e++;
	return (true);
}

/**
 * \brief add nb_e elements of size sz_e from the array arr, if needed grows the
 * da
 * \param da pointer to the dynamic array
 * \param arr pointer to the array to concatenate
 * \param nb_e number of elements to add
 * \param sz_e size of the elements
 * \return true if success, false if failure
 **/
bool da_cat_array(t_da *da, void *arr, size_t nb_e, size_t sz_e) {
	void  *tmp;
	size_t new_cap;

	if (!arr || !da || !sz_e || !da->array) return (false);
	if (sz_e != da->sz_e) return (false);
	new_cap = da->cap_e;
	while ((da->nb_e + nb_e) > da->cap_e) {
		da_grow(da);
	}
	memcpy(da_at(da, da->nb_e), arr, da->sz_e * nb_e);
	da->nb_e += nb_e;
	return (true);
}
