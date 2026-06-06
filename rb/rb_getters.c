#include "round_buffer.h"

// BUG: check math, i_pos+1 is not wrapped in % nb_max so it might lead to BS.
/**
 * \brief true if rb is full, false otherwise
 * \param rb the round buffer
 * \return true if full, false otherwise
**/
bool rb_full(t_rb rb) {
	return (rb.o_pos == rb.i_pos + 1);
}
/**
 * \brief true if rb is empty, false otherwise
 * \param rb the round buffer
 * \return true if empty, false otherwise
**/
bool rb_empty(t_rb rb) {
	return (rb.i_pos == rb.o_pos);
}
/**
 * \brief return number of entries in rb not yet read/poped
 * \param rb the round buffer
 * \return number of elements in the buffer
**/
size_t rb_size(t_rb rb) {
	if (rb.o_pos <= rb.i_pos) return (rb.i_pos - rb.o_pos);
	return ((rb.nb_max - rb.o_pos) + rb.i_pos);
}
/**
 * \brief return max number of elements rb can hold
 * \param rb the round buffer
 * \return the maximum capacity
**/
size_t rb_cappacity(t_rb rb) {
	return (rb.nb_max);
}
/**
 * \brief return number of elements rb left as disponible to hold
 * \param rb the round buffer
 * \return number of free slots
**/
size_t rb_free(t_rb rb) {
	return (rb_cappacity(rb) - rb_size(rb));
}
