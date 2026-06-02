#include <string.h>
#include "./round_buffer.h"

void rb_push(t_rb *rb, const void *data) {
	if (!rb) return;
	memcpy(rb->array + (rb->i_pos * rb->sz_e), data, rb->sz_e);
	if ((rb->i_pos + 1) % rb->nb_max == rb->o_pos) {
		rb->o_pos = (rb->o_pos + 1) % rb->nb_max;
	}
	rb->i_pos = (rb->i_pos + 1) % rb->nb_max;
}
void rb_npush(t_rb *rb, const void *data, size_t n) {
	if (!rb) return ;
	while (n > rb->nb_max) {
		data = (unsigned char *)data + (rb->nb_max * rb->sz_e);
		n -= rb->nb_max;
	}
	while (n)
		rb_push(rb, data);
}
bool rb_push_safe(t_rb *rb, const void *data) {
	if (!rb) return (false);
	if ((rb->i_pos + 1) % rb->nb_max == rb->o_pos) return (false);
	return (rb_push(rb, data), true);
}
bool rb_npush_safe(t_rb *rb, const void *data, size_t n) {
	size_t i =0;

	if (!rb) return (false);
	if (n > rb->nb_max) return (false);
	if ((rb->i_pos + n) % rb->nb_max == rb->o_pos) return (false);
	while (i < n)
		rb_push(rb, data + i * rb->sz_e);
	return (true);
}
