#include <string.h>

#include "round_buffer.h"

/**
 * \brief gets the first element that matches the comparator, copies it to
 * buffer
 * \param rb the round buffer
 * \param comp the comparator function
 * \param arg argument for the comparator
 * \param buffer out buffer where the found element will be copied
 * \return pointer to the out buffer if found, NULL otherwise
 * \warning rb_get assume that buffer is always correctly sized.
**/
void *rb_get(const t_rb *rb, t_comparator comp, const void *arg, void *buffer) {
	void *elem;

	if (!rb || !comp || !buffer) return NULL;

	elem = rb_find(rb, comp, arg);
	if (elem) return (memcpy(buffer, elem, rb->sz_e));
	return NULL;
}

/**
 * \brief gets up to n elements starting from the first match, copies them to
 * buffer
 * \param rb the round buffer
 * \param n maximum number of elements to copy
 * \param comp the comparator function
 * \param arg argument for the comparator
 * \param buffer out buffer where the elements will be copied
 * \return pointer to the out buffer if match found, NULL otherwise
 * \warning rb_get assume that buffer is always correctly sized.
**/
void *rb_nget(const t_rb *rb, size_t n, t_comparator comp, const void *arg,
              void *buffer) {
	size_t i = 0;
	size_t pos;
	size_t count;
	void  *elem;

	if (!rb || !comp || !buffer || !n) return NULL;

	count = rb_size(*rb);
	pos   = rb->o_pos;
	while (i < count) {
		elem = rb->array + (pos * rb->sz_e);
		if (comp(elem, (void *)arg) == 0) {
			if (rb_nat(*rb, i, n, buffer)) return buffer;
			return NULL;
		}
		pos = (pos + 1) % rb->nb_max;
		i++;
	}
	return NULL;
}
