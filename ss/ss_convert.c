#include "sparse_set.h"

/**
 * \brief Allocate and copy all elements from ss into a new array.
 * \param ss sparse set to copy from
 * \return new array of elem
**/
void *ss_to_array(t_ss *ss) {
	void *ret;

	if (!ss || !ss->data) return (NULL);
	ret = malloc(ss->nb_elem * ss->sz_elem);
	if (!ret) return (NULL);
	return (ss_to_loc_array(ss, ret));
}
/**
 * \brief copies values to a given array. Array is expected of holding nb_elem.
 * \param ss sparse set to copy from
 * \param arr array to copy to
 * \return arr
**/
void *ss_to_loc_array(t_ss *ss, void *arr) {
	if (!ss || !arr) return (NULL);
	for (size_t i = 0; i < ss->nb_elem; i++)
		ss_get(ss, i, (arr + i * ss->sz_elem));
	return (arr);
}
