#include <string.h>

#define C_MINI__RB_INTERNAL
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
void *rb_get(const t_cm_rb *rb, t_cm_comparator comp, const void *arg,
             void *buffer) {
	const void *elem;

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
void *rb_nget(const t_cm_rb *rb, size_t n, t_cm_comparator comp,
              const void *arg, void *buffer) {
	size_t i = 0;
	size_t pos;
	size_t count;
	void  *elem;

	if (!rb || !comp || !buffer || !n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return NULL;
	}

	count = rb_size(*rb);
	pos   = rbi_opos_get(rb);
	while (i < count) {
		elem = rbi_get_pos(rb, pos);
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
// note: rb_nat at line 17 of funct: no fail branch since rb_nat only checks
//	{buff, idx, rb}. No fail condition possible otherwise. They have been
//  checked above in the function

/**
 * \brief gets up to n elements that match the comparator, copies them to
 * buffer
 * \param rb the round buffer
 * \param n maximum number of matching elements to copy
 * \param comp the comparator function
 * \param arg argument for the comparator
 * \param buffer out buffer where the matching elements will be copied
 * \return pointer to the out buffer if at least one match found, NULL otherwise
 * \warning rb_getn assume that buffer is always correctly sized.
 * \ingroup rb
**/
void *rb_getn(const t_cm_rb *rb, size_t n, t_cm_comparator comp,
              const void *arg, void *buffer) {
	size_t i = 0;
	size_t pos;
	size_t count;
	size_t found = 0;
	void  *elem;

	if (!rb || !comp || !buffer || !n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return NULL;
	}

	count = rb_size(*rb);
	pos   = rbi_opos_get(rb);
	while (i < count && found < n) {
		elem = rbi_get_pos(rb, pos);
		if (comp(elem, (void *)arg) == 0) {
			memcpy((unsigned char *)buffer + (found * rb->sz_e), elem,
			       rb->sz_e);
			found++;
		}
		pos = (pos + 1) % rb->nb_max;
		i++;
	}
	if (found) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return buffer;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_FAIL)
	return NULL;
}
