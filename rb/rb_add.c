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
	if (!rb || !data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return;
	}
	memcpy(rbi_get_pos(rb, rbi_ipos_get(rb)), data, rb->sz_e);
	rbi_ipos_set(rb, (rbi_ipos_get(rb) + 1) % rb->nb_max);
	if (rbi_count_get(rb) == rb->nb_max)
		rbi_opos_set(rb, (rbi_opos_get(rb) + 1) % rb->nb_max);
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

	if (!rb || !data) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return;
	}
	sdata = data;
	if (n > rb_capacity(*rb)) {
		sdata += rb->sz_e * (n - rb_capacity(*rb));
		n      = rb_capacity(*rb);
	}
	for (size_t i = 0; i < n; i++)
		rb_push(rb, sdata + (i * rb->sz_e));
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
	size_t i = 0;

	if (!rb || !data || n > rb->nb_max) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (false);
	}
	if (rb_size(*rb) + n > rb_capacity(*rb)) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RB_LOG_ESPACE)
		return (false);
	}
	while (i < n)
		rb_push(rb, (unsigned char *)data + i++ * rb->sz_e);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (true);
}
