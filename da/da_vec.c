#include <stdint.h>
#include <string.h>

#include "dynamic_array.h"

/**
 * \brief number of elements currently stored in the dynamic array
 * \param da pointer to the dynamic array
 * \return the element count, or 0 when da is NULL
 * \ingroup da
**/
size_t da_size(const t_cm_da *da) {
	return (da ? da->nb_e : 0);
}

/**
 * \brief number of elements the dynamic array can hold without growing
 * \param da pointer to the dynamic array
 * \return the capacity, or 0 when da is NULL
 * \ingroup da
**/
size_t da_capacity(const t_cm_da *da) {
	return (da ? da->cap_e : 0);
}

/**
 * \brief pointer to the first element of the dynamic array
 * \param da pointer to the dynamic array
 * \return pointer to the first element, or NULL when empty or invalid
 * \ingroup da
**/
void *da_front(const t_cm_da *da) {
	if (!da_is_valid(da) || !da->nb_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (da_at(da, 0));
}

/**
 * \brief pointer to the last element of the dynamic array
 * \param da pointer to the dynamic array
 * \return pointer to the last element, or NULL when empty or invalid
 * \ingroup da
**/
void *da_back(const t_cm_da *da) {
	if (!da_is_valid(da) || !da->nb_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (da_at(da, da->nb_e - 1));
}

/**
 * \brief ensure the dynamic array can hold at least n elements
 * \param da pointer to the dynamic array
 * \param n requested capacity
 * \return true on success, false on failure
 * \note the number of stored elements is left untouched
 * \ingroup da
**/
bool da_reserve(t_cm_da *da, size_t n) {
	if (!da_is_not_null(da)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if (n <= da->cap_e) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
		return (true);
	}
	if (n > SIZE_MAX / da->sz_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if (!da_grow_for_size(da, n - da->nb_e)) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC)
		return (false);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}

/**
 * \brief resize the dynamic array to n elements
 * \param da pointer to the dynamic array
 * \param n new number of stored elements
 * \param fill pointer to the value copied into every newly created element. Use
 * NULL to zero-fill them
 * \return true on success, false on failure
 * \note shrinking only lowers the element count, the capacity is left as is
 * (unless auto-shrink is enabled)
 * \ingroup da
**/
bool da_resize(t_cm_da *da, size_t n, const void *fill) {
	size_t old_nb;
	size_t i;
	char  *base;

	if (!da_is_not_null(da)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if (n > da->cap_e) {
		if (n > SIZE_MAX / da->sz_e) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
			return (false);
		}
		if (!da_grow_for_size(da, n - da->nb_e)) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC)
			return (false);
		}
	}
	old_nb = da->nb_e;
	base   = (char *)da->array;
	if (n > old_nb) {
		if (fill) {
			for (i = old_nb; i < n; i++)
				memcpy(base + (i * da->sz_e), fill, da->sz_e);
		} else {
			memset(base + (old_nb * da->sz_e), 0, (n - old_nb) * da->sz_e);
		}
	}
	da->nb_e = n;
	if (n < old_nb) (void)da_shrink_auto(da);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}

/**
 * \brief insert an element at pos, shifting the following elements right
 * \param da pointer to the dynamic array
 * \param pos index where the element is inserted (pos == nb_e appends)
 * \param elem pointer to the element to insert
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_insert(t_cm_da *da, size_t pos, const void *elem) {
	char *base;

	if (!da_is_not_null(da) || !elem || pos > da->nb_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if ((da->nb_e + 1) > da->cap_e && !da_grow_for_size(da, 1)) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC)
		return (false);
	}
	base = (char *)da->array;
	if (pos < da->nb_e)
		memmove(base + ((pos + 1) * da->sz_e), base + (pos * da->sz_e),
		        (da->nb_e - pos) * da->sz_e);
	memcpy(base + (pos * da->sz_e), elem, da->sz_e);
	da->nb_e++;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}

/**
 * \brief erase one element at pos, keeping the relative order of the others
 * \param da pointer to the dynamic array
 * \param pos index of the element to erase
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_erase(t_cm_da *da, size_t pos) {
	if (!da_is_valid(da) || pos >= da->nb_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	return (da_rm_order(da, pos));
}
