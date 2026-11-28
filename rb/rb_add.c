
#include <string.h>

#define C_MINI__RB_INTERNAL
#include "round_buffer.h"

/**
 * \brief add value to rb, if overwrite happens, silently resolves and continues
 * \param rb the round buffer
 * \param data the element to push
 * \ingroup rb
**/
void rb_push(t_cm_rb *rb, const void *data) {
	size_t i_pos;

	if (!rb || !data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return;
	}
	i_pos = rbi_ipos_get(rb);
	memcpy(rbi_get_pos(rb, i_pos), data, rb->sz_e);
	rbi_ipos_set(rb, rbi_wrap(rb, i_pos + 1));
	if (rbi_count_get(rb) == rb->nb_max)
		rbi_opos_set(rb, rbi_wrap(rb, rbi_opos_get(rb) + 1));
	else rbi_count_inc(rb);

	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
}
/**
 * \brief add values to rb, if overwrite happens, silently resolves and
 * continues
 * \param rb the round buffer
 * \param data the elements to push
 * \param n number of elements to push
 * \ingroup rb
**/
void rb_npush(t_cm_rb *rb, const void *data, size_t n) {
	const unsigned char *sdata;
	size_t               cap;
	size_t               count;
	size_t               i_pos;
	size_t               drop;

	if (!rb || !data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return;
	}
	cap   = rb->nb_max;
	sdata = data;
	if (n > cap) {
		sdata += rb->sz_e * (n - cap);
		n      = cap;
	}
	if (!n) return;
	i_pos = rbi_ipos_get(rb);
	rbi_copy_in(rb, i_pos, sdata, n);
	if (n == cap) {
		// the whole buffer is replaced, oldest element is the old input head
		rbi_opos_set(rb, i_pos);
		rbi_count_set(rb, cap);
	} else {
		count = rbi_count_get(rb);
		drop  = (count + n > cap) ? count + n - cap : 0;
		if (drop) {
			rbi_opos_set(rb, rbi_wrap(rb, rbi_opos_get(rb) + drop));
			count -= drop;
		}
		rbi_ipos_set(rb, rbi_wrap(rb, i_pos + n));
		rbi_count_set(rb, count + n);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
}
/**
 * \brief add value to rb, fails on overwrite if it would happen
 * \param rb the round buffer
 * \param data the element to push
 * \return true on success, false if overwrite would happen
 * \ingroup rb
**/
bool rb_push_safe(t_cm_rb *rb, const void *data) {
	if (!rb || !data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (false);
	}
	if (rb_size(*rb) == rb_capacity(*rb)) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RB_LOG_ESPACE)
		return (false);
	}
	rb_push(rb, data);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (true);
}
/**
 * \brief add values to rb, fails on overwrite if it would happen
 * \param rb the round buffer
 * \param data the elements to push
 * \param n number of elements to push
 * \return true on success, false if overwrite would happen
 * \ingroup rb
**/
bool rb_npush_safe(t_cm_rb *rb, const void *data, size_t n) {
	if (!rb || !data || n > rb->nb_max) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (false);
	}
	if (rb_size(*rb) + n > rb_capacity(*rb)) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RB_LOG_ESPACE)
		return (false);
	}
	rb_npush(rb, data, n);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (true);
}
