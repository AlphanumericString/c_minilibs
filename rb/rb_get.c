#include <string.h>

#include "round_buffer.h"

/**
 * \brief gets the first element that matches the comparator, copies it to
 * buffer
 * \param rb the round buffer
 * \param comp the comparator function
 * \param arg argument for the comparator
 * \param buffer out buffer where the found element will be copied
 * \return pointer to the out buffer if found, NULL otherwise
 * \warning rb_get assume that buffer is always correctly sized.
 * \ingroup rb
**/
void *rb_get(const t_rb *rb, t_cm_comparator comp, const void *arg,
             void *buffer) {
	void *elem;

	if (!rb || !comp || !buffer) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return NULL;
	}
	elem = rb_find(rb, comp, arg);
	if (elem) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (memcpy(buffer, elem, rb->sz_e));
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_FAIL)
	// zero out here
	return NULL;
}

/**
 * \brief gets up to n elements starting from the first match, copies them to
 * buffer
 * \param rb the round buffer
 * \param n maximum number of elements to copy
 * \param comp the comparator function
 * \param arg argument for the comparator
 * \param buffer out buffer where the elements will be copied
 * \return pointer to the out buffer if match found, NULL otherwise
 * \warning rb_get assume that buffer is always correctly sized.
 * \ingroup rb
**/
void *rb_nget(const t_rb *rb, size_t n, t_cm_comparator comp, const void *arg,
              void *buffer) {
	size_t i = 0;
	size_t pos;
	size_t count;
	void  *elem;

	if (!rb || !comp || !buffer || !n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return NULL;
	}

	count = rb_size(*rb);
	pos   = rb->o_pos;
	while (i < count) {
		elem = rb->array + (pos * rb->sz_e);
		if (comp(elem, (void *)arg) == 0) {
			rb_nat(*rb, i, n, buffer);
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
			return buffer;
		}
		pos = (pos + 1) % rb->nb_max;
		i++;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_FAIL)
	return NULL;
}
// note: rb_nat at line 18 of funct: no fail branch since rb_nat only checks
//	buff, idx and rb. No fail condition possible otherwise
