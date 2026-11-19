#include "dynamic_array.h"

/**
 * \brief pull iterator over the elements in [start, last[
 * \param da pointer to the dynamic array (the iterator handle)
 * \param start index of the first element to yield
 * \param last index of the end element (exclusive). Use -1 (SIZE_MAX) or any
 * value out of bounds to iterate up to and including the last element
 * \param it_val element pointer used as the iteration state. It must be
 * initialized to NULL before the first call; on a true return it holds the
 * address of the yielded element
 * \return true while an element is available, false once the range is
 * exhausted or on invalid arguments
 * \note typical usage:
 * \code
 * void  *it_val = NULL;
 * size_t start  = 0;
 * while (da_iter(&da, start, (size_t)-1, &it_val))
 *     do_something(*(int *)it_val);
 * \endcode
 * \ingroup da
**/
bool da_iter(const t_cm_da *da, size_t start, size_t last, void **it_val) {
	char *next;
	char *end;

	if (!da_is_valid(da) || da_is_empty(da) || !it_val) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		if (it_val) *it_val = NULL;
		return (false);
	}
	if (last == (size_t)-1 || last > da->nb_e) last = da->nb_e;
	if (start >= last) {
		*it_val = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
		return (false);
	}
	if (!*it_val) {
		*it_val = da_at(da, start);
	} else {
		next = (char *)*it_val + da->sz_e;
		end  = (char *)da->array + (last * da->sz_e);
		if (next >= end) {
			*it_val = NULL;
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
			return (false);
		}
		*it_val = next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}

/**
 * \brief reverse pull iterator over the elements in [start, last[
 * \param da pointer to the dynamic array (the iterator handle)
 * \param start index of the first element of the range. It is the last element
 * yielded by the iterator
 * \param last index of the end element (exclusive). Use -1 (SIZE_MAX) or any
 * value out of bounds to iterate from the last element of the da
 * \param it_val element pointer used as the iteration state. It must be
 * initialized to NULL before the first call; on a true return it holds the
 * address of the yielded element
 * \return true while an element is available, false once the range is
 * exhausted or on invalid arguments
 * \note the elements are yielded from last - 1 down to start included
 * \note typical usage:
 * \code
 * void  *it_val = NULL;
 * size_t start  = 0;
 * while (da_iter_rev(&da, start, (size_t)-1, &it_val))
 *     do_something(*(int *)it_val);
 * \endcode
 * \ingroup da
**/
bool da_iter_rev(const t_cm_da *da, size_t start, size_t last, void **it_val) {
	char *prev;
	char *begin;

	if (!da_is_valid(da) || da_is_empty(da) || !it_val) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		if (it_val) *it_val = NULL;
		return (false);
	}
	if (last == (size_t)-1 || last > da->nb_e) last = da->nb_e;
	if (start >= last) {
		*it_val = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
		return (false);
	}
	if (!*it_val) {
		*it_val = da_at(da, last - 1);
	} else {
		begin = (char *)da->array + (start * da->sz_e);
		if ((char *)*it_val == begin) {
			*it_val = NULL;
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
			return (false);
		}
		prev    = (char *)*it_val - da->sz_e;
		*it_val = prev;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}

/**
 * \brief advance an element pointer to the next element of the dynamic array
 * \param da pointer to the dynamic array
 * \param it iterator holding the current element address. Initialize it to NULL
 * to yield the first element
 * \return the next element address, or NULL when the end is reached (in which
 * case *it is reset to NULL)
 * \ingroup da
**/
void *da_it_advance(const t_cm_da *da, void **it) {
	char *base;
	char *end;
	char *next;

	if (!da_is_valid(da) || da_is_empty(da) || !it) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		if (it) *it = NULL;
		return (NULL);
	}
	base = (char *)da->array;
	end  = base + (da->nb_e * da->sz_e);
	if (!*it) {
		*it = base;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
		return (*it);
	}
	next = (char *)*it + da->sz_e;
	if (next >= end) {
		*it = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
		return (NULL);
	}
	*it = next;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (*it);
}

/**
 * \brief move an element pointer to the previous element of the dynamic array
 * \param da pointer to the dynamic array
 * \param it iterator holding the current element address. Initialize it to NULL
 * to yield the last element
 * \return the previous element address, or NULL when the start is reached (in
 * which case *it is reset to NULL)
 * \ingroup da
**/
void *da_it_back(const t_cm_da *da, void **it) {
	char *base;
	char *end;
	char *prev;

	if (!da_is_valid(da) || da_is_empty(da) || !it) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		if (it) *it = NULL;
		return (NULL);
	}
	base = (char *)da->array;
	end  = base + (da->nb_e * da->sz_e);
	if (!*it) {
		*it = end - da->sz_e;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
		return (*it);
	}
	if ((char *)*it == base) {
		*it = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
		return (NULL);
	}
	prev = (char *)*it - da->sz_e;
	*it  = prev;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (*it);
}
