#define C_MINI__HP_INTERNAL

#include <stdlib.h>

#include "heap.h"

/**
 * \brief map every element of the heap through a function and store the
 * returned pointers in a new array
 * \param h heap pointer
 * \param fn function transforming an element, must return a non-NULL pointer
 * (NULL aborts the mapping)
 * \return a newly allocated array of h->nb pointers, or NULL on empty heap,
 * invalid arguments, callback failure or allocation failure
 * \note the returned array preserves the heap backing-array order, so
 * out[i] is the value returned for the element at backing-array index i.
 * The array only owns the pointers, not the data they point to; free it with
 * free when done.
 * \ingroup hp
**/
void **hp_map(const t_cm_hp *h, t_cm_mapper fn) {
	void **out;

	if (!hp_is_valid(h) || !fn) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	if (hp_is_empty(h)) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
		return (NULL);
	}
	out = malloc(h->nb * sizeof(*out));
	if (!out) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__HP_LOG_EALLOC)
		return (NULL);
	}
	for (size_t i = 0; i < h->nb; i++) {
		out[i] = fn(hpi_elem_ptr(h, i));
		if (!out[i]) {
			CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_CB_FAIL, i)
			free(out);
			return (NULL);
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (out);
}
