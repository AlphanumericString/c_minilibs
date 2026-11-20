#define C_MINI__HP_INTERNAL

#include <stdlib.h>
#include <string.h>

#include "heap.h"

/**
 * \brief get the root element of the heap without removing it
 * \param h heap pointer
 * \return pointer to the root element or NULL if the heap is empty or invalid
 * \ingroup hp
**/
void *hp_peek(const t_cm_hp *h) {
	if (!hp_is_valid(h) || hp_is_empty(h)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hpi_elem_ptr(h, 0));
}

/**
 * \brief For each element in the heap, apply a callback receiving its position
 * and whether it is the last element
 * \param h heap pointer
 * \param cb callback function
 * \param data user argument
 * \return true if all callbacks returned true, false otherwise
 * \ingroup hp
**/
bool hp_foreach_it(const t_cm_hp *h, t_cm_callback_it cb, void *data) {
	if (!h || !cb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	for (size_t i = 0; i < h->nb; i++)
		if (cb(hpi_elem_ptr(h, i), data, i, i + 1 == h->nb) == false) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_CB_FAIL, i)
			return (false);
		}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}

/**
 * \brief find an element matching the condition
 * \param h heap pointer
 * \param c callback to test elements, equality is a zero return
 * \param data user data for callback
 * \return pointer to the found element, or NULL if not found
 * \note the scan is linear: element order is the backing-array order
 * \ingroup hp
**/
void *hp_find(const t_cm_hp *h, t_cm_comparator c, void *data) {
	if (!hp_is_valid(h) || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	for (size_t i = 0; i < h->nb; i++)
		if (c(hpi_elem_ptr(h, i), data) == 0) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
			return (hpi_elem_ptr(h, i));
		}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (NULL);
}

/**
 * \brief find an element matching the condition and copy it to a buffer
 * \param h heap pointer
 * \param c callback to test elements, equality is a zero return
 * \param data user data for callback
 * \param out_buffer buffer receiving the found element
 * \return true if found and copied, false otherwise
 * \ingroup hp
**/
bool hp_find_get(const t_cm_hp *h, t_cm_comparator c, void *data,
                 void *out_buffer) {
	const void *elem;

	if (!hp_is_valid(h) || !c || !out_buffer) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	elem = hp_find(h, c, data);
	if (!elem) return (false);
	memcpy(out_buffer, elem, h->sz_e);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}

/**
 * \brief fold the elements of the heap into a single value
 * \param h heap pointer
 * \param m merger function, called as m(element, accumulator)
 * \param data starting value (also the accumulator)
 * \return the folded value, or NULL on invalid arguments
 * \ingroup hp
**/
void *hp_fold(const t_cm_hp *h, t_cm_merger m, void *data) {
	if (!hp_is_valid(h) || !m || !data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	for (size_t i = 0; i < h->nb; i++)
		data = m(hpi_elem_ptr(h, i), data);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (data);
}

/**
 * \brief reduce the elements of the heap into a newly allocated value
 * \param h heap pointer
 * \param m merger function, called as m(element, accumulator)
 * \return the reduced value or NULL on invalid arguments or allocation failure
 * \note the returned value is allocated with calloc and belongs to the caller
 * \ingroup hp
**/
void *hp_reduce(const t_cm_hp *h, t_cm_merger m) {
	void *bag;

	if (!hp_is_valid(h) || !m) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	bag = calloc(1, h->sz_e);
	if (!bag) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__HP_LOG_EALLOC)
		return (NULL);
	}
	for (size_t i = 0; i < h->nb; i++)
		bag = m(hpi_elem_ptr(h, i), bag);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (bag);
}
