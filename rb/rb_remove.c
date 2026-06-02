#include <string.h>
#include "./round_buffer.h"

bool rb_drop(t_rb *rb, size_t n) {
	if (!rb || !n) return (false);
	if (n > rb->nb_max ||
	    (rb->o_pos < rb->i_pos && ((rb->o_pos + n) % rb->nb_max) > rb->i_pos) ||
	    (rb->o_pos > rb->i_pos && ((rb->o_pos + n) % rb->nb_max) < rb->i_pos)) {
		return (rb_clr(rb), true);
	}
	rb->o_pos = (rb->o_pos + n) % rb->nb_max;
	return (true);
}
void *rb_peek(t_rb rb, void *elem_out) {
	unsigned char *pos;

	pos = rb.array + (rb.o_pos * rb.sz_e);
	if (elem_out) memcpy(elem_out, pos, rb.sz_e);
	return (pos);
}
void *rb_npeek(t_rb rb, size_t n, void *elem_out) {
	unsigned char *pos;
	size_t         left_size;

	if (!n) return (NULL);
	if (n > rb.nb_max) n = rb.nb_max;
	if (n > rb_size(rb)) {
		n = rb_size(rb);
	}
	pos = rb.array + (rb.o_pos * rb.sz_e);
	if (elem_out) {
		if (rb.nb_max > (rb.o_pos + n)) {
			memcpy(elem_out, pos, n * rb.sz_e);
		} else {
			left_size = (rb.nb_max - rb.o_pos) * rb.sz_e;
			memcpy(elem_out, pos, left_size);
			n  -= rb.nb_max - rb.o_pos;
			pos = rb.array;
			memcpy(elem_out + left_size, pos, n * rb.sz_e);
		}
	}
	return (pos);
}
void *rb_pop(t_rb *rb, void *elem_out) {
	void *return_val;

	if (!rb) return (NULL);
	return_val = rb_peek(*rb, elem_out);
	rb->o_pos  = (rb->o_pos + 1) % rb->nb_max;
	return (return_val);
}
void *rb_npop(t_rb *rb, size_t n, void *elem_out) {
	void *return_val;

	if (!rb) return (NULL);
	return_val = rb_npeek(*rb, n, elem_out);
	if (n > rb_size(*rb)) n = rb_size(*rb);
	rb->o_pos = (rb->o_pos + n) % rb->nb_max;
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
