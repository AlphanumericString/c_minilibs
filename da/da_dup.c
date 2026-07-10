#include <string.h>

#include "dynamic_array.h"

/**
 * \brief duplicate a dynamic array
 * \param src pointer to the source dynamic array
 * \return the duplicated dynamic array
**/
t_da da_dup(const t_da *src) {
	return (da_ndup(src, -1));
}

/**
 * \brief duplicate the first n elements of a dynamic array
 * \param src pointer to the source dynamic array
 * \param n number of elements to duplicate
 * \return the duplicated dynamic array
**/
t_da da_ndup(const t_da *src, size_t n) {
	t_da ret = {.array = NULL, .nb_e = 0, .cap_e = 0, .sz_e = 0};

	if (!src || !n) return (ret);
	if (n > src->nb_e) n = src->nb_e;
	if (!da_ninit(&ret, n, src->sz_e)) return (ret);
	ret.nb_e = n;
	return (memcpy(ret.array, src->array, n * src->sz_e), ret);
}

/**
 * \brief duplicate a dynamic array into an existing one
 * \param dst pointer to the destination dynamic array
 * \param src pointer to the source dynamic array
 * \return pointer to the destination dynamic array
**/
t_da *da_dup_to(t_da *dst, const t_da *src) {
	return (da_ndup_to(dst, -1, src));
}

/**
 * \brief duplicate the first n elements of a dynamic array into an existing one
 * \param dst pointer to the destination dynamic array
 * \param n number of elements to duplicate
 * \param src pointer to the source dynamic array
 * \return pointer to the destination dynamic array
**/
t_da *da_ndup_to(t_da *dst, size_t n, const t_da *src) {
	void *pos;

	if (!src || !dst || !n) return (dst);
	if (n > src->nb_e) n = src->nb_e;
	if (dst->cap_e - dst->nb_e < n) da_grow_for_size(dst, n);
	dst->nb_e += n;
	pos        = da_at(dst, dst->nb_e - n);
	return (memcpy(pos, src->array, n * src->sz_e), dst);
}
