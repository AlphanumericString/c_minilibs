#include "./dynamic_array.h"

void *da_to_array(t_da *vec) {
	void *data;

	if (!vec) return (NULL);
	data = malloc(vec->sz_e * vec->nb_e);
	if (!data) return (NULL);
	return (da_to_loc_array(vec, data));
}
void *da_to_loc_array(t_da *vec, void *arr) {
	if (!vec || !arr) return (NULL);
	memcpy(arr, vec->array, vec->sz_e * vec->nb_e);
	return (arr);
}
