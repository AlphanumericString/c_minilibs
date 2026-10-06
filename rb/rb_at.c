#include <string.h>

#include "round_buffer.h"

/**
 * \brief returns the pointer of element at index idx (from o_pos)
 * \param rb the round buffer
 * \param idx the index from o_pos
 * \return pointer to the element in the data array, or NULL if out of bounds
 * \ingroup rb
**/
void *rb_at(t_rb rb, size_t idx) {
	size_t pos;

	if (idx >= rb_size(rb)) {
		LOG(C_MINI__RB_LOG_EARG);
		return NULL;
	}
	pos = (rb.o_pos + idx) % rb.nb_max;
	LOG(C_MINI__RB_LOG_SUCCESS);
	return rb.array + (pos * rb.sz_e);
}
/**
 * \brief return position in array for element at index and copies up to n
 * elements
 * \param rb the round buffer
 * \param idx the index from o_pos
 * \param n maximum number of elements to copy
 * \param buff out buffer to copy elements to
 * \return pointer to the first element in the data array, or NULL on error
 * \warning rb_nat MAKES NO PROMISES that the returned adress is contigous on n.
 * \warning rb_nat assume that buff is always correctly sized.
 * \ingroup rb
**/
void *rb_nat(t_rb rb, size_t idx, size_t n, void *buff) {
	size_t start_pos;
	size_t max;
	size_t first_chunk;
	size_t second_chunk;

	if (idx >= rb_size(rb) || !n || !buff) {
		LOG(C_MINI__RB_LOG_EARG);
		return NULL;
	}
	max = rb_size(rb);
	if (n > max) n = max;
	start_pos = (rb.o_pos + idx) % rb.nb_max;
	if (start_pos + n <= rb.nb_max) {
		memcpy(buff, rb.array + (start_pos * rb.sz_e), n * rb.sz_e);
	} else {
		first_chunk  = rb.nb_max - start_pos;
		second_chunk = n - first_chunk;
		memcpy(buff, rb.array + (start_pos * rb.sz_e), first_chunk * rb.sz_e);
		memcpy(buff + (first_chunk * rb.sz_e), rb.array,
		       second_chunk * rb.sz_e);
	}
	LOG(C_MINI__RB_LOG_SUCCESS);
	return rb.array + (start_pos * rb.sz_e);
}
