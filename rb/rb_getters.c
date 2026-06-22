#include "round_buffer.h"

/**
 * \brief true if rb is full, false otherwise
 * \param rb the round buffer
 * \return true if full, false otherwise
**/
bool rb_full(t_rb rb) {
	return (rb.count == rb.nb_max);
}
/**
 * \brief true if rb is empty, false otherwise
 * \param rb the round buffer
 * \return true if empty, false otherwise
**/
bool rb_empty(t_rb rb) {
	return (rb.count == 0);
}
/**
 * \brief return number of entries in rb not yet read/poped
 * \param rb the round buffer
 * \return number of elements in the buffer
**/
size_t rb_size(t_rb rb) {
	return (rb.count);
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
