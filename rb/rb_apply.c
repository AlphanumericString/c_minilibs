#include "round_buffer.h"

/**
 * \brief apply callback to up to n elements
 * \param rb the round buffer
 * \param n the maximum number of elements to apply to
 * \param cb the callback function
 * \param arg argument for the callback
 * \return false if the callback returns false, true otherwise
 * \ingroup rb
**/
bool rb_napply(t_rb *rb, size_t n, t_callback cb, const void *arg) {
	size_t i = 0;
	size_t current_pos;

	if (!rb || !cb) {
		LOG(C_MINI__RB_LOG_EARG);
		return false;
	}
	if (n > rb_size(*rb)) n = rb_size(*rb);
	current_pos = rb->o_pos;
	while (i < n) {
		if (!cb(rb->array + (current_pos * rb->sz_e), (void *)arg)) {
			LOG(C_MINI__RB_LOG_ECB);
			return false;
		}
		current_pos = (current_pos + 1) % rb->nb_max;
		i++;
	}
	LOG(C_MINI__RB_LOG_SUCCESS);
	return true;
}

/**
 * \brief apply callback to all elements
 * \param rb the round buffer
 * \param cb the callback function
 * \param arg argument for the callback
 * \return false if the callback returns false, true otherwise
 * \ingroup rb
**/
bool rb_apply(t_rb *rb, t_callback cb, const void *arg) {
	bool r;

	if (!rb || !cb) {
		LOG(C_MINI__RB_LOG_EARG);
		return false;
	}
	r = rb_napply(rb, (size_t)-1, cb, arg);
	if (!r) {
		LOG(C_MINI__RB_LOG_FAIL);
		return (r);
	}
	LOG(C_MINI__RB_LOG_SUCCESS);
	return (r);
}
