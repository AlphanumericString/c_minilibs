#define C_MINI__RB_INTERNAL
#include "round_buffer.h"

/**
 * \brief returns the pointer of element at index idx (from o_pos)
 * \param rb the round buffer
 * \param idx the index from o_pos
 * \return pointer to the element in the data array, or NULL if out of bounds
 * \note on a magic buffer the returned pointer is directly usable without
 * wrapping
 * \ingroup rb
**/
void *rb_at(t_cm_rb rb, size_t idx) {
	if (idx >= rb_size(rb)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return NULL;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return rbi_get_pos(&rb, rbi_head_off(&rb, idx));
}
/**
 * \brief return position in array for element at index and copies up to n
 * elements
 * \param rb the round buffer
 * \param idx the index from o_pos
 * \param n maximum number of elements to copy
 * \param buff out buffer to copy elements to
 * \return pointer to the first element in the data array, or NULL on error
 * \warning rb_nat only guarantees that buff receives n contiguous elements. On
 * a magic buffer the returned array pointer is contiguous too, on a
 * rb_from_array buffer it wraps and must not be read past the window.
 * \warning rb_nat assumes that buff is always correctly sized.
 * \note n is clipped to the number of elements left after idx.
 * \ingroup rb
**/
void *rb_nat(t_cm_rb rb, size_t idx, size_t n, void *buff) {
	size_t pos;
	size_t left;

	if (idx >= rb_size(rb) || !n || !buff) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return NULL;
	}
	left = rb_size(rb) - idx;
	if (n > left) n = left;
	pos = rbi_head_off(&rb, idx);
	rbi_copy_out(&rb, pos, n, buff);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return rbi_get_pos(&rb, pos);
}
