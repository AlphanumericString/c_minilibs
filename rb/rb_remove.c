#include <string.h>

#define C_MINI__RB_INTERNAL
#include "round_buffer.h"

/**
 * \brief removes n elements
 * \param rb the round buffer
 * \param n number of elements to drop
 * \return true if removes n elements, false otherwise
 * \ingroup rb
**/
bool rb_drop(t_rb *rb, size_t n) {
	if (!rb || !n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (false);
	}
	if (n >= rbi_count_get(rb)) {
		rb_clr(rb);
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (true);
	}
	rbi_opos_set(rb, (rbi_opos_get(rb) + n) % rb->nb_max);
	rbi_count_set(rb, rbi_count_get(rb) - n);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (true);
}
/**
 * \brief return pointer to the next element, if elem_out is given copy the
 * value to it
 * \param rb the round buffer
 * \param elem_out pointer where the peeked element will be copied (optional)
 * \return pointer to the element in the buffer
 * \ingroup rb
**/
void *rb_peek(t_rb rb, void *elem_out) {
	unsigned char *pos;

	pos = rb_at(rb, 0);
	if (elem_out) {
		if (pos) {
			memcpy(elem_out, pos, rb.sz_e);
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		} else {
			bzero(elem_out, rb.sz_e);
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_FAIL)
		}
	}
	return (pos);
}
/**
 * \brief return pointer to the next n elements, if elem_out is given copy the
 * values to it
 * \param rb the round buffer
 * \param n number of elements to peek
 * \param elem_out pointer where the elements will be copied (optional)
 * \return pointer to the first element in the buffer
 * \ingroup rb
**/
void *rb_npeek(t_rb rb, size_t n, void *elem_out) {
	unsigned char *pos;
	size_t         left_size;

	if (!rb.array) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_EARG)
		return (NULL);
	}
	if (n > rb_size(rb)) n = rb_size(rb);
	if (!n) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		return (NULL);
	}
	pos = rb_at(rb, 0);
	if (elem_out) {
		if (rb.nb_max > (rbi_opos_get(&rb) + n)) {
			memcpy(elem_out, pos, n * rb.sz_e);
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		} else {
			left_size = (rb.nb_max - rbi_opos_get(&rb)) * rb.sz_e;
			memcpy(elem_out, pos, left_size);
			n -= rb.nb_max - rbi_opos_get(&rb);
			memcpy((unsigned char *)elem_out + left_size, rb.array,
			       n * rb.sz_e);
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
		}
	}
	return (pos);
}
/**
 * \brief returns removed element position, if elem_out copy removed elem to it
 * \param rb the round buffer
 * \param elem_out pointer where the removed element will be copied (optional)
 * \return pointer to the removed element in the buffer
 * \ingroup rb
**/
void *rb_pop(t_rb *rb, void *elem_out) {
	void *return_val;

	if (!rb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (NULL);
	}
	return_val = rb_peek(*rb, elem_out);
	if (!return_val) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_FAIL)
		return (return_val);
	}
	rbi_opos_set(rb, (rbi_opos_get(rb) + 1) % rb->nb_max);
	rbi_count_dec(rb);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (return_val);
}
/**
 * \brief returns first removed element position, if elem_out copy values to it
 * \param rb the round buffer
 * \param n number of elements to pop
 * \param elem_out pointer where the elements will be copied (optional)
 * \return pointer to the first removed element in the buffer
 * \ingroup rb
**/
void *rb_npop(t_rb *rb, size_t n, void *elem_out) {
	void *return_val;

	if (!rb || !n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (NULL);
	}
	if (n > rb_size(*rb)) n = rb_size(*rb);
	return_val = rb_npeek(*rb, n, elem_out);
	if (!return_val) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_FAIL)
		return (return_val);
	}
	rbi_opos_set(rb, (rbi_opos_get(rb) + n) % rb->nb_max);
	rbi_count_set(rb, rbi_count_get(rb) - n);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (return_val);
}
