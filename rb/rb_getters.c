#define C_MINI__RB_INTERNAL
#include "round_buffer.h"

/**
 * \brief true if rb is full, false otherwise
 * \param rb the round buffer
 * \return true if full, false otherwise (also false if !array)
 * \ingroup rb
**/
bool rb_full(t_rb rb) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (rb.array && rbi_count_get(&rb) == rb.nb_max);
}
/**
 * \brief true if rb is empty, false otherwise
 * \param rb the round buffer
 * \return true if empty or no array, false otherwise
 * \ingroup rb
**/
bool rb_empty(t_rb rb) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (!rb.array || rbi_count_get(&rb) == 0);
}
/**
 * \brief return number of entries in rb not yet read/poped
 * \param rb the round buffer
 * \return number of elements in the buffer
 * \ingroup rb
**/
size_t rb_size(t_rb rb) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (rbi_count_get(&rb));
}
/**
 * \brief return max number of elements rb can hold
 * \param rb the round buffer
 * \return the maximum capacity
 * \ingroup rb
**/
size_t rb_capacity(t_rb rb) {
	if (!rb.array) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (rb.nb_max);
}
/**
 * \brief return number of elements rb has left available to hold
 * \param rb the round buffer
 * \return number of free slots
 * \ingroup rb
**/
size_t rb_remaining_space(t_rb rb) {
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (rb_capacity(rb) - rb_size(rb));
}
