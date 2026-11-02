#include <stdlib.h>

#define C_MINI__RB_INTERNALS
#include "round_buffer.h"

/**
 * \brief populate rb's field and allocate array from sz_e and nb_e
 * \param rb the round buffer to populate
 * \param sz_elem size of each element
 * \param nb_e number of elements
 * \return the populated round buffer or NULL on failure
 * \ingroup rb
**/
t_rb *rb_create(t_rb *rb, size_t sz_elem, size_t nb_e) {
	if (!rb || !sz_elem || !nb_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (NULL);
	}
	rb->array = malloc(sz_elem * nb_e);
	if (!rb->array) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RB_LOG_EALLOC)
		return (NULL);
	}
	rb->sz_e   = sz_elem;
	rb->nb_max = nb_e;
	rbi_opos_set(rb, 0);
	rbi_ipos_set(rb, 0);
	rbi_count_set(rb, 0);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (rb);
}
/**
 * \brief populate rb's field using the default nb_e and given sz_e. allocate
 * array
 * \param rb the round buffer to populate
 * \param sz_elem size of each element
 * \return the populated round buffer or NULL on failure
 * \ingroup rb
**/
t_rb *rb_init(t_rb *rb, size_t sz_elem) {
	t_rb *ret;

	ret = rb_create(rb, sz_elem, C_MINI__RB_NBE_DFLT);
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
 * \ingroup rb
**/
t_rb *rb_from_array(t_rb *rb, void *array, size_t arr_cap, size_t sz_elem) {
	if (!rb || !array || !arr_cap || !sz_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (NULL);
	}
	// if (arr_cap == (size_t)-1)
	// 	for (arr_cap = 0; ((void **)array)[arr_cap] != NULL; arr_cap++)
	// 		;
	rbi_ipos_set(rb, 0);
	rbi_opos_set(rb, 0);
	rb->sz_e   = sz_elem;
	rb->nb_max = arr_cap;
	rb->array  = array;
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
t_rb *rb_new(size_t sz_elem) {
	t_rb *ret;

	if (!sz_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (NULL);
	}
	ret = malloc(sizeof(t_rb));
	if (rb_init(ret, sz_elem) == NULL) {
		if (ret) free(ret);
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RB_LOG_EALLOC)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (ret);
}
