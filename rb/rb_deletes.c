#include <stdlib.h>

#include "round_buffer.h"

/**
 * \brief clears inner buffer and reset to 0 all inner fields. does not free rb
 * \param rb the round buffer to destroy
 **/
void rb_destroy(t_rb *rb) {
	if (!rb) {
		LOG(C_MINI__RB_LOG_EARG);
		return;
	}
	if (rb->array) free(rb->array);
	*rb = (t_rb){0};
	LOG(C_MINI__RB_LOG_SUCCESS);
}
/**
 * \brief reset i_pos and o_pos, effectively setting all buffer as ok o push to
 * \param rb the round buffer to clear
 **/
void rb_clr(t_rb *rb) {
	if (!rb) {
		LOG(C_MINI__RB_LOG_EARG);
		return;
	}
	rb->o_pos = 0;
	rb->i_pos = rb->o_pos;
	rb->count = 0;
	LOG(C_MINI__RB_LOG_SUCCESS);
}
/**
 * \brief free's the rb inner and outer
 * \param rb the round buffer to free
 **/
void rb_free(t_rb *rb) {
	if (!rb) {
		LOG(C_MINI__RB_LOG_EARG);
		return;
	}
	rb_destroy(rb);
	free(rb);
	LOG(C_MINI__RB_LOG_SUCCESS);
}
