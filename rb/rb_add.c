#include <stdio.h>
#include <string.h>

#include "round_buffer.h"

/**
 * \brief add value to rb, if overwrite happens, silently resolve and contiues
 * \param rb the round buffer
 * \param data the element to push
 **/
void rb_push(t_rb *rb, const void *data) {
	if (!rb) return;
	memcpy(rb->array + (rb->i_pos * rb->sz_e), data, rb->sz_e);
	if ((rb->i_pos + 1) % rb->nb_max == rb->o_pos) {
		rb->o_pos = (rb->o_pos + 1) % rb->nb_max;
	}
	rb->i_pos = (rb->i_pos + 1) % rb->nb_max;
}
/**
 * \brief add values to rb, if overwrite happens, silently resolve and contiues
 * \param rb the round buffer
 * \param data the elements to push
 * \param n number of elements to push
 **/
void rb_npush(t_rb *rb, const void *data, size_t n) {
	if (!rb) return;
	if (n > rb_free(*rb)) n = rb_free(*rb);
	printf("n:%zu\n", n);
	for (size_t i = 0; i < n; i++)
		rb_push(rb, (const char *)data + (i * rb->sz_e));
}
/**
 * \brief add value to rb, fails on overwrite if it would happen
 * \param rb the round buffer
 * \param data the element to push
 * \return true on success, false if overwrite would happen
 **/
bool rb_push_safe(t_rb *rb, const void *data) {
	if (!rb) return (false);
	if ((rb->i_pos) % rb->nb_max == rb->o_pos) return (false);
	return (rb_push(rb, data), true);
}
/**
 * \brief add values to rb, fails on overwrite if it would happen
 * \param rb the round buffer
 * \param data the elements to push
 * \param n number of elements to push
 * \return true on success, false if overwrite would happen
 **/
bool rb_npush_safe(t_rb *rb, const void *data, size_t n) {
	size_t i = 0;

	if (!rb) return (false);
	if (n > rb->nb_max) return (false);
	if ((rb->i_pos + n) % rb->nb_max == rb->o_pos) return (false);
	while (i < n)
		rb_push(rb, data + i++ * rb->sz_e);
	return (true);
}
