#include "./round_buffer.h"

void rb_destroy(t_rb *rb) {
	if (!rb) return;
	if (rb->array) free(rb->array);
	*rb = (t_rb){0};
}
void rb_clr(t_rb *rb) {
	if (!rb) return;
	rb->o_pos = 0;
	rb->i_pos = rb->o_pos;
}
