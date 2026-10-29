#include <stdio.h>
#include <string.h>

#include "dynamic_array.h"

/**
 * \brief duplicate a dynamic array
 * \param src pointer to the source dynamic array
 * \return the duplicated dynamic array
 * \ingroup da
**/
t_da da_dup(const t_da *src) {
	t_da r;

	if (!da_is_valid(src)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return ((t_da){0});
	}
	r = da_ndup(src, -1);
	if (r.array) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	} else {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC)
	}
	return (r);
}
/**
 * \brief duplicate the first n elements of a dynamic array
 * \param src pointer to the source dynamic array
 * \param n number of elements to duplicate
 * \return the duplicated dynamic array
 * \ingroup da
**/
t_da da_ndup(const t_da *src, size_t n) {
	t_da ret = {.array = NULL, .nb_e = 0, .cap_e = 0, .sz_e = 0};

	if (!da_is_valid(src) || !n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return ((t_da){0});
	}
	if (n > src->nb_e) n = src->nb_e;
	if (!da_ninit(&ret, n, src->sz_e)) return (ret);
	ret.nb_e = n;
	memcpy(ret.array, src->array, n * src->sz_e);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (ret);
}
/**
 * \brief duplicate a dynamic array into an existing one
 * \param dst pointer to the destination dynamic array
 * \param src pointer to the source dynamic array
 * \return pointer to the destination dynamic array
 * \ingroup da
**/
t_da *da_dup_to(t_da *dst, const t_da *src) {
	t_da *ret;

	if (!dst || !da_is_valid(src)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (NULL);
	}
	ret = da_ndup_to(dst, -1, src);
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC)
		return ((t_da *)NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (ret);
}
/**
 * \brief duplicate the first n elements of a dynamic array into an existing one
 * \param dst pointer to the destination dynamic array
 * \param n number of elements to duplicate
 * \param src pointer to the source dynamic array
 * \return pointer to the destination dynamic array
 * \ingroup da
**/
t_da *da_ndup_to(t_da *dst, size_t n, const t_da *src) {
	void *pos;

	if (!da_is_valid(src) || !da_is_valid(dst)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (NULL);
	}
	if (!n) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
		return (dst);
	}
	if (n > src->nb_e) n = src->nb_e;
	if ((dst->cap_e - dst->nb_e < n) && (da_grow_for_size(dst, n) == false)) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC)
		return ((t_da *)NULL);
	}
	dst->nb_e += n;
	pos        = da_at(dst, dst->nb_e - n);
	memcpy(pos, src->array, n * src->sz_e);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (dst);
}
