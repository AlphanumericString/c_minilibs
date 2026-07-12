#include <string.h>

#include "round_buffer.h"

/**
 * \brief removes n elements
 * \param rb the round buffer
 * \param n number of elements to drop
 * \return true if removes n elements, false otherwise
 **/
bool rb_drop(t_rb *rb, size_t n) {
	if (!rb || !n) {
		LOG(C_MINI__RB_LOG_EARG);
		return (false);
	}
	if (n >= rb->count) {
		rb_clr(rb);
		LOG(C_MINI__RB_LOG_SUCCESS);
		return (true);
	}
	rb->o_pos  = (rb->o_pos + n) % rb->nb_max;
	rb->count -= n;
	LOG(C_MINI__RB_LOG_SUCCESS);
	return (true);
}
/**
 * \brief return position in array for next elem, if elem_out copies value to it
 * \param rb the round buffer
 * \param elem_out pointer where the removed element will be copied (optional)
 * \return pointer to the element in the buffer
 **/
void *rb_peek(t_rb rb, void *elem_out) {
	unsigned char *pos;

	pos = rb_at(rb, 0);
	if (elem_out) {
		if (pos) {
			memcpy(elem_out, pos, rb.sz_e);
			LOG(C_MINI__RB_LOG_SUCCESS);
		} else {
			bzero(elem_out, rb.sz_e);
			LOG(C_MINI__RB_LOG_FAIL);
		}
	}
	return (pos);
}
/**
 * \brief return position in array for next elems, if elem_out copies values to
 * it
 * \param rb the round buffer
 * \param n number of elements to peek
 * \param elem_out pointer where the elements will be copied (optional)
 * \return pointer to the first element in the buffer
 **/
void *rb_npeek(t_rb rb, size_t n, void *elem_out) {
	unsigned char *pos;
	size_t         left_size;

	if (n > rb_size(rb)) n = rb_size(rb);
	if (!n) {
		LOG(C_MINI__RB_LOG_SUCCESS);
		return (NULL);
	}
	pos = rb_at(rb, 0);
	if (elem_out) {
		if (!pos) {
			bzero(elem_out, n * rb.sz_e);
			LOG(C_MINI__RB_LOG_FAIL);
		} else if (rb.nb_max > (rb.o_pos + n)) {
			memcpy(elem_out, pos, n * rb.sz_e);
			LOG(C_MINI__RB_LOG_SUCCESS);
		} else {
			left_size = (rb.nb_max - rb.o_pos) * rb.sz_e;
			memcpy(elem_out, pos, left_size);
			n  -= rb.nb_max - rb.o_pos;
			pos = rb.array;
			memcpy(elem_out + left_size, pos, n * rb.sz_e);
			LOG(C_MINI__RB_LOG_SUCCESS);
		}
	}
	return (pos);
}
/**
 * \brief returns removed element position, if elem_out copy removed elem to it
 * \param rb the round buffer
 * \param elem_out pointer where the removed element will be copied (optional)
 * \return pointer to the removed element in the buffer
 **/
void *rb_pop(t_rb *rb, void *elem_out) {
	void *return_val;

	if (!rb) {
		LOG(C_MINI__RB_LOG_EARG);
		return (NULL);
	}
	return_val = rb_peek(*rb, elem_out);
	rb->o_pos  = (rb->o_pos + 1) % rb->nb_max;
	rb->count--;
	LOG(C_MINI__RB_LOG_SUCCESS);
	return (return_val);
}
/**
 * \brief returns first removed element position, if elem_out copy values to it
 * \param rb the round buffer
 * \param n number of elements to pop
 * \param elem_out pointer where the elements will be copied (optional)
 * \return pointer to the first removed element in the buffer
 **/
void *rb_npop(t_rb *rb, size_t n, void *elem_out) {
	void *return_val;

	if (!rb || !n) {
		LOG(C_MINI__RB_LOG_EARG);
		return (NULL);
	}
	if (n > rb_size(*rb)) n = rb_size(*rb);
	return_val = rb_npeek(*rb, n, elem_out);
	rb->o_pos  = (rb->o_pos + n) % rb->nb_max;
	rb->count -= n;
	LOG(C_MINI__RB_LOG_SUCCESS);
	return (return_val);
}

// // NOTE:
// // (rb.o_pos - rb.nb_max + n) > rb.i_pos)
// // calc with arr=[i_pos 0 1 o_pos] n=2
// // o_pos - 4 + 2 > 0 ?
// // 3-4+2 > 0
// // 1 > 0
// static bool would_cross(t_rb rb, size_t n) {
// 	if ((rb.o_pos < rb.i_pos && (rb.o_pos + n) > rb.i_pos) ||
// 	    (rb.o_pos > rb.i_pos && (rb.o_pos - rb.nb_max + n) > rb.i_pos))
// 		return (true);
// 	return (false);
// }
