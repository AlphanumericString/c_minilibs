#include <stdint.h>
#include <stdlib.h>

#define C_MINI__RB_INTERNAL
#include "round_buffer.h"

/**
 * \brief populate rb's field and allocate the backing array from sz_e and nb_e
 * \param rb the round buffer to populate
 * \param sz_elem size of each element
 * \param nb_e number of elements
 * \param require_magic when true, fail instead of falling back to malloc
 * \return the populated round buffer or NULL on failure
 * \ingroup rb
**/
static t_cm_rb *rbi_create(t_cm_rb *rb, size_t sz_elem, size_t nb_e,
                           bool require_magic) {
	void  *array    = NULL;
	size_t map_size = 0;
	size_t nb_max   = nb_e;
	size_t nb_bytes;

	if (!rb || !sz_elem || !nb_e || nb_e > SIZE_MAX / sz_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (NULL);
	}
	nb_bytes = nb_e * sz_elem;
	if (rbi_map_ring(nb_bytes, &array, &map_size)) {
		// the mirror window must be a whole number of pages, capacity grows
		nb_max       = map_size / sz_elem;
		rb->mirrored = true;
	} else {
		if (require_magic) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RB_LOG_EALLOC)
			return (NULL);
		}
		array = malloc(nb_bytes);
		if (!array) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RB_LOG_EALLOC)
			return (NULL);
		}
		rb->mirrored = false;
	}
	rb->array    = array;
	rb->map_size = map_size;
	rb->sz_e     = sz_elem;
	rb->nb_max   = nb_max;
	rbi_opos_set(rb, 0);
	rbi_ipos_set(rb, 0);
	rbi_count_set(rb, 0);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (rb);
}

/**
 * \brief populate rb's field and allocate the backing array from sz_e and nb_e.
 * Tries a magic (double mapped) buffer and falls back on malloc when the
 * mapping is unavailable.
 * \param rb the round buffer to populate
 * \param sz_elem size of each element
 * \param nb_e number of elements (magic buffers are rounded up to a page)
 * \return the populated round buffer or NULL on failure
 * \note use rb_create_magic for a hard failure on mapping errors
 * \ingroup rb
**/
t_cm_rb *rb_create(t_cm_rb *rb, size_t sz_elem, size_t nb_e) {
	return (rbi_create(rb, sz_elem, nb_e, false));
}

/**
 * \brief populate rb's field and allocate a magic (double mapped) backing array
 * from sz_e and nb_e
 * \param rb the round buffer to populate
 * \param sz_elem size of each element
 * \param nb_e number of elements (capacity is rounded up to a page)
 * \return the populated round buffer or NULL if the mapping fails
 * \ingroup rb
**/
t_cm_rb *rb_create_magic(t_cm_rb *rb, size_t sz_elem, size_t nb_e) {
	return (rbi_create(rb, sz_elem, nb_e, true));
}

/**
 * \brief populate rb's field using the default nb_e and given sz_e, allocate
 * a magic array when possible
 * \param rb the round buffer to populate
 * \param sz_elem size of each element
 * \return the populated round buffer or NULL on failure
 * \ingroup rb
**/
t_cm_rb *rb_init(t_cm_rb *rb, size_t sz_elem) {
	t_cm_rb *ret;

	ret = rb_create(rb, sz_elem, C_MINI__RB_NBE_DFLT);
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_FAIL)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (ret);
}

/**
 * \brief populate rb's field using the default nb_e and given sz_e, allocate a
 * magic array
 * \param rb the round buffer to populate
 * \param sz_elem size of each element
 * \return the populated round buffer or NULL on failure
 * \ingroup rb
**/
t_cm_rb *rb_init_magic(t_cm_rb *rb, size_t sz_elem) {
	t_cm_rb *ret;

	ret = rb_create_magic(rb, sz_elem, C_MINI__RB_NBE_DFLT);
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_FAIL)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (ret);
}

/**
 * \brief populate rb from given values, does not allocate array
 * \param rb the round buffer to populate
 * \param array the pre-allocated array to use
 * \param arr_cap number of elements the array can hold
 * \param sz_elem size of each element
 * \return the populated round buffer
 * \note the buffer takes ownership of array: rb_destroy will free() it. Pass
 * heap memory, never a stack array or a pointer into another object.
 * \note the array is not mirrored, so this buffer keeps the two chunks
 * copy paths (no contiguous wrap-around).
 * \ingroup rb
**/
t_cm_rb *rb_from_array(t_cm_rb *rb, void *array, size_t arr_cap,
                       size_t sz_elem) {
	if (!rb || !array || !arr_cap || !sz_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (NULL);
	}
	rbi_ipos_set(rb, 0);
	rbi_opos_set(rb, 0);
	rb->sz_e     = sz_elem;
	rb->nb_max   = arr_cap;
	rb->array    = array;
	rb->map_size = 0;
	rb->mirrored = false;
	rbi_count_set(rb, 0);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (rb);
}

/**
 * \brief allocate a new rb and populate it with default nb_e and given sz_e
 * \param sz_elem size of each element
 * \return the newly allocated round buffer or NULL on failure
 * \ingroup rb
**/
t_cm_rb *rb_new(size_t sz_elem) {
	t_cm_rb *ret;

	if (!sz_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (NULL);
	}
	ret = malloc(sizeof(t_cm_rb));
	if (rb_init(ret, sz_elem) == NULL) {
		if (ret) free(ret);
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RB_LOG_EALLOC)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (ret);
}

/**
 * \brief allocate a new rb and populate it with default nb_e and given sz_e,
 * backing array must be magic
 * \param sz_elem size of each element
 * \return the newly allocated round buffer or NULL on failure
 * \ingroup rb
**/
t_cm_rb *rb_new_magic(size_t sz_elem) {
	t_cm_rb *ret;

	if (!sz_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (NULL);
	}
	ret = malloc(sizeof(t_cm_rb));
	if (rb_init_magic(ret, sz_elem) == NULL) {
		if (ret) free(ret);
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RB_LOG_EALLOC)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (ret);
}
