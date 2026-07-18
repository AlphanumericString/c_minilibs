#include "round_buffer.h"

/**
 * \brief true if rb is full, false otherwise
 * \param rb the round buffer
 * \return true if full, false otherwise
 * \ingroup rb
**/
bool rb_full(t_rb rb) {
	LOG(C_MINI__RB_LOG_SUCCESS);
	return (rb.count == rb.nb_max);
}
/**
 * \brief true if rb is empty, false otherwise
 * \param rb the round buffer
 * \return true if empty, false otherwise
 * \ingroup rb
**/
bool rb_empty(t_rb rb) {
	LOG(C_MINI__RB_LOG_SUCCESS);
	return (rb.count == 0);
}
/**
 * \brief return number of entries in rb not yet read/poped
 * \param rb the round buffer
 * \return number of elements in the buffer
 * \ingroup rb
**/
size_t rb_size(t_rb rb) {
	LOG(C_MINI__RB_LOG_SUCCESS);
	return (rb.count);
}
/**
 * \brief return max number of elements rb can hold
 * \param rb the round buffer
 * \return the maximum capacity
 * \ingroup rb
**/
size_t rb_capacity(t_rb rb) {
	LOG(C_MINI__RB_LOG_SUCCESS);
	return (rb.nb_max);
}
/**
 * \brief return number of elements rb left as disponible to hold
 * \param rb the round buffer
 * \return number of free slots
 * \ingroup rb
**/
size_t rb_remaining_space(t_rb rb) {
	LOG(C_MINI__RB_LOG_SUCCESS);
	return (rb_capacity(rb) - rb_size(rb));
}
