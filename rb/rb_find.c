#include "round_buffer.h"

/**
 * \brief finds the first element that matches the comparator
 * \param rb the round buffer
 * \param c the comparator function
 * \param a argument for the comparator
 * \return pointer to the element in the data array, or NULL if not found
**/
void *rb_find(const t_rb *rb, t_comparator c, const void *a) {
	size_t i = 0;
	size_t pos;
	size_t count;
	void  *elem;

	if (!rb || !c) return NULL;

	count = rb_size(*rb);
	pos   = rb->o_pos;
	while (i < count) {
		elem = rb->array + (pos * rb->sz_e);
		if (c(elem, (void *)a) == 0) {
			return elem;
		}
		pos = (pos + 1) % rb->nb_max;
		i++;
	}
	return NULL;
}
