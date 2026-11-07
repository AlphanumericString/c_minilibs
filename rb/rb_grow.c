#include <stdlib.h>
#include <string.h>

#define C_MINI__RB_INTERNAL
#include "round_buffer.h"

/**
 * \brief duplicates the round buffer with nb_elem_added new empty slots
 * \param rb the old round buffer
 * \param nb_elem_added number of additional elements
 * \return newly allocated t_cm_rb or NULL if alloc fails
 * \note yields a newly grown rb to not invalidate i_pos and o_pos
 * \ingroup rb
**/
t_cm_rb *rb_grow(const t_cm_rb *rb, size_t nb_elem_added) {
	t_cm_rb *new_rb;

	if (!rb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return NULL;
	}
	new_rb = malloc(sizeof(t_cm_rb));
	if (!rb_create(new_rb, rb->sz_e, rb->nb_max + nb_elem_added)) {
		if (new_rb) free(new_rb);
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RB_LOG_EALLOC)
		return NULL;
	}
	rb_nat(*rb, 0, -1, new_rb->array);
	rbi_opos_set(new_rb, 0);
	rbi_count_set(new_rb, rb_size(*rb));
	rbi_ipos_set(new_rb, rbi_count_get(new_rb));
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return new_rb;
}
