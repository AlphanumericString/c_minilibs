#include "./dynamic_array.h"

void *da_at(t_da *vec, size_t pos) {
	if (!vec || pos > vec->nb_e) return (NULL);
	return ((char *)vec->array + vec->sz_e * pos);
}

bool da_set(t_da *vec, size_t pos, void *elem) {
	if (!vec || pos >= vec->nb_e) return (false);
	return (memcpy(vec->array + vec->sz_e * pos, elem, vec->sz_e), true);
}

bool da_get(t_da *vec, size_t pos, void *elem) {
	if (!vec || pos >= vec->nb_e) return (false);
	if (!elem) return (false);
	return (memcpy(elem, da_at(vec, pos), vec->sz_e), true);
}
void *da_array(t_da *da) {
	if (!da) return (NULL);
	return (da->array);
}
