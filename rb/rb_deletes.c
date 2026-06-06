#include "round_buffer.h"

/**
 * \brief clears inner buffer and reset to 0 all inner fields. does not free rb
 * \param rb the round buffer to destroy
**/
void rb_destroy(t_rb *rb) {
	if (!rb) return;
	if (rb->array) free(rb->array);
	*rb = (t_rb){0};
}
/**
 * \brief reset i_pos and o_pos, effectively setting all buffer as ok o push to
 * \param rb the round buffer to clear
**/
void rb_clr(t_rb *rb) {
	if (!rb) return;
	rb->o_pos = 0;
	rb->i_pos = rb->o_pos;
}
