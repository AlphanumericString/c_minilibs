#include <string.h>

#include "round_buffer.h"

/**
 * \brief apply duplicator to up to n elements and store in out_buff
 * \param rb the round buffer
 * \param n maximum number of elements to map
 * \param dup the duplicator function
 * \param out_buff array where the address of the duplicated elements are copied
 * \return true on success, false if an element couldn't be duplicated
 * \ingroup rb
**/
bool rb_nmap(t_rb *rb, size_t n, t_cm_duplicator dup, void **out_buff) {
	size_t i = 0;
	size_t pos;
	size_t count;
	void  *elem;
	void  *dup_elem;

	if (!rb || !dup || !out_buff || !n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (false);
	}

	count = rb_size(*rb);
	if (n > count) n = count;

	pos = rb->o_pos;
	while (i < n) {
		elem     = rb->array + (pos * rb->sz_e);
		dup_elem = dup(elem);
		if (dup_elem)
			memcpy(((unsigned char *)out_buff) + (i * sizeof(void *)),
			       &dup_elem, sizeof(void *));
		else {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_ECB)
			return (bzero(((unsigned char *)out_buff) + (i * sizeof(void *)),
			              sizeof(void *)),
			        false);
		}
		pos = (pos + 1) % rb->nb_max;
		i++;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (true);
}

/**
 * \brief apply duplicator to all elements and store in out_buff
 * \param rb the round buffer
 * \param dup the duplicator function
 * \param out_buff array where the address of the duplicated elements are copied
 * \return true on success, false if an element couldn't be duplicated
 * \ingroup rb
**/
bool rb_map(t_rb *rb, t_cm_duplicator dup, void **out_buff) {
	bool r;

	if (!rb || !dup || !out_buff) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (false);
	}
	r = rb_nmap(rb, (size_t)-1, dup, out_buff);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (r);
}
