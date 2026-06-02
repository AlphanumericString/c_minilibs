#include "./dynamic_array.h"

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
