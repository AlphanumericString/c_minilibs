#include <stdlib.h>

#define C_MINI__RB_INTERNAL
#include "round_buffer.h"

/**
 * \brief frees the inner buffer and resets all inner fields to 0. does not free
 * the rb struct itself
 * \param rb the round buffer to destroy
 * \ingroup rb
**/
void rb_destroy(t_cm_rb *rb) {
	if (!rb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return;
	}
	if (rb->array) free(rb->array);
	rb->array  = NULL;
	rb->nb_max = 0;
	rb->sz_e   = 0;
	rbi_count_set(rb, 0);
	rbi_ipos_set(rb, 0);
	rbi_opos_set(rb, 0);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
}
/**
 * \brief reset i_pos and o_pos, effectively setting the whole buffer as ok to
 * push to
 * \param rb the round buffer to clear
 * \ingroup rb
**/
void rb_clr(t_cm_rb *rb) {
	if (!rb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return;
	}
	rbi_opos_set(rb, 0);
	rbi_ipos_set(rb, 0);
	rbi_count_set(rb, 0);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
}
/**
 * \brief free's the rb inner and outer
 * \param rb the round buffer to free
 * \ingroup rb
**/
void rb_free(t_cm_rb **rb) {
	if (!rb || !*rb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return;
	}
	rb_destroy(*rb);
	free(*rb);
	*rb = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
}
