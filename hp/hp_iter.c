#define C_MINI__HP_INTERNAL

#include "heap.h"

/**
 * \internal
 * \brief level-order pull iterator over the elements in [start, last[
 * \param h heap pointer
 * \param start index of the first element to yield
 * \param last index of the end element (exclusive)
 * \param it_val element pointer used as the iteration state
 * \return true while an element is available, false otherwise
 * \ingroup hp
**/
static bool hpi_iter_level(const t_cm_hp *h, size_t start, size_t last,
                           void **it_val) {
	char       *next;
	const char *end;

	if (!hp_is_valid(h) || hp_is_empty(h) || !it_val) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		if (it_val) *it_val = NULL;
		return (false);
	}
	if (last == (size_t)-1 || last > h->nb) last = h->nb;
	if (start >= last) {
		*it_val = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
		return (false);
	}
	if (!*it_val) {
		*it_val = hpi_elem_ptr(h, start);
	} else {
		next = (char *)*it_val + h->sz_e;
		end  = (char *)h->elems + (last * h->sz_e);
		if (next >= end) {
			*it_val = NULL;
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
			return (false);
		}
		*it_val = next;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}

/**
 * \internal
 * \brief reverse level-order pull iterator over the elements in [start, last[
 * \param h heap pointer
 * \param start index of the first element of the range (last yielded)
 * \param last index of the end element (exclusive)
 * \param it_val element pointer used as the iteration state
 * \return true while an element is available, false otherwise
 * \ingroup hp
**/
static bool hpi_iter_level_rev(const t_cm_hp *h, size_t start, size_t last,
                               void **it_val) {
	const char *begin;

	if (!hp_is_valid(h) || hp_is_empty(h) || !it_val) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		if (it_val) *it_val = NULL;
		return (false);
	}
	if (last == (size_t)-1 || last > h->nb) last = h->nb;
	if (start >= last) {
		*it_val = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
		return (false);
	}
	if (!*it_val) {
		*it_val = hpi_elem_ptr(h, last - 1);
	} else {
		begin = hpi_elem_ptr(h, start);
		if ((char *)*it_val == begin) {
			*it_val = NULL;
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
			return (false);
		}
		*it_val = (char *)*it_val - h->sz_e;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}

/**
 * \brief pull iterator over the elements in [start, last[ in level order
 * \param h pointer to the heap (the iterator handle)
 * \param start index of the first element to yield
 * \param last index of the end element (exclusive). Use -1 (SIZE_MAX) or any
 * value out of bounds to iterate up to and including the last element
 * \param it_val element pointer used as the iteration state. It must be
 * initialized to NULL before the first call; on a true return it holds the
 * address of the yielded element
 * \return true while an element is available, false once the range is
 * exhausted or on invalid arguments
 * \note the yielded order is the heap backing-array order, not sorted order.
 * Use repeated pops (on a duplicate if needed) for sorted traversal.
 * \note typical usage:
 * \code
 * void  *it_val = NULL;
 * size_t start  = 0;
 * while (hp_iter(&h, start, (size_t)-1, &it_val))
 *     do_something(*(int *)it_val);
 * \endcode
 * \ingroup hp
**/
bool hp_iter(const t_cm_hp *h, size_t start, size_t last, void **it_val) {
	return (hpi_iter_level(h, start, last, it_val));
}

/**
 * \brief reverse pull iterator over the elements in [start, last[
 * \param h pointer to the heap (the iterator handle)
 * \param start index of the first element of the range. It is the last element
 * yielded by the iterator
 * \param last index of the end element (exclusive). Use -1 (SIZE_MAX) or any
 * value out of bounds to iterate from the last element of the heap
 * \param it_val element pointer used as the iteration state. It must be
 * initialized to NULL before the first call; on a true return it holds the
 * address of the yielded element
 * \return true while an element is available, false once the range is
 * exhausted or on invalid arguments
 * \note the elements are yielded from last - 1 down to start included, in
 * backing-array order
 * \ingroup hp
**/
bool hp_iter_rev(const t_cm_hp *h, size_t start, size_t last, void **it_val) {
	return (hpi_iter_level_rev(h, start, last, it_val));
}

/**
 * \brief advance an element pointer to the next element of the heap
 * \param h pointer to the heap
 * \param it iterator holding the current element address. Initialize it to
 * NULL to yield the first element
 * \return the next element address, or NULL when the end is reached (in which
 * case *it is reset to NULL)
 * \ingroup hp
**/
void *hp_it_advance(const t_cm_hp *h, void **it) {
	char       *base;
	const char *end;
	char       *next;

	if (!hp_is_valid(h) || hp_is_empty(h) || !it) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		if (it) *it = NULL;
		return (NULL);
	}
	base = (char *)h->elems;
	end  = base + (h->nb * h->sz_e);
	if (!*it) {
		*it = base;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
		return (*it);
	}
	next = (char *)*it + h->sz_e;
	if (next >= end) {
		*it = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
		return (NULL);
	}
	*it = next;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (*it);
}

/**
 * \brief move an element pointer to the previous element of the heap
 * \param h pointer to the heap
 * \param it iterator holding the current element address. Initialize it to
 * NULL to yield the last element
 * \return the previous element address, or NULL when the start is reached (in
 * which case *it is reset to NULL)
 * \ingroup hp
**/
void *hp_it_back(const t_cm_hp *h, void **it) {
	char *base;
	char *prev;

	if (!hp_is_valid(h) || hp_is_empty(h) || !it) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		if (it) *it = NULL;
		return (NULL);
	}
	base = (char *)h->elems;
	if (!*it) {
		*it = base + ((h->nb - 1) * h->sz_e);
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
		return (*it);
	}
	if ((char *)*it == base) {
		*it = NULL;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
		return (NULL);
	}
	prev = (char *)*it - h->sz_e;
	*it  = prev;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (*it);
}
