#include "./round_buffer.h"

// getters
bool rb_full(t_rb rb) {
	return (rb.o_pos == rb.i_pos + 1);
}
bool rb_empty(t_rb rb) {
	return (rb.i_pos == rb.o_pos);
}
size_t rb_size(t_rb rb) {
	if (rb.o_pos <= rb.i_pos) return (rb.i_pos - rb.o_pos);
	return ((rb.nb_max - rb.o_pos) + rb.i_pos);
}
size_t rb_cappacity(t_rb rb) {
	return (rb.nb_max);
}
size_t rb_free(t_rb rb) {
	return (rb_cappacity(rb) - rb_size(rb));
}
