#include <stdlib.h>
#include <string.h>
#include "./round_buffer.h"

/**
 * \brief grows the round buffer by nb_elem_added elements
 * \param rb the old round buffer
 * \param nb_elem_added number of additional elements
 * \return newly allocated t_rb or NULL if alloc fails
 **/
t_rb *rb_grow(t_rb *rb, size_t nb_elem_added) {
	t_rb *new_rb;
	size_t count;
	size_t first_part;

	if (!rb) return NULL;
	new_rb = malloc(sizeof(t_rb));
	if (!new_rb) return NULL;

	if (!rb_create(new_rb, rb->sz_e, rb->nb_max + nb_elem_added)) {
		free(new_rb);
		return NULL;
	}

	count = rb_size(*rb);
	if (count > 0) {
		if (rb->o_pos <= rb->i_pos) {
			memcpy(new_rb->array, rb->array + (rb->o_pos * rb->sz_e), count * rb->sz_e);
		} else {
			first_part = rb->nb_max - rb->o_pos;
			memcpy(new_rb->array, rb->array + (rb->o_pos * rb->sz_e), first_part * rb->sz_e);
			memcpy(new_rb->array + (first_part * rb->sz_e), rb->array, rb->i_pos * rb->sz_e);
		}
	}
	new_rb->o_pos = 0;
	new_rb->i_pos = count;
	return new_rb;
}