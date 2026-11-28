#include <string.h>

#define C_MINI__RB_INTERNAL
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
bool rb_nmap(const t_cm_rb *rb, size_t n, t_cm_duplicator dup,
             void **out_buff) {
	size_t      i = 0;
	size_t      base;
	size_t      count;
	void       *elem;
	const void *dup_elem;

	if (!rb || !dup || !out_buff || !n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (false);
	}

	count = rb_size(*rb);
	if (n > count) n = count;

	base = rbi_opos_get(rb);
	while (i < n) {
		elem     = rbi_get_pos(rb, rbi_off_at(rb, base, i));
		dup_elem = dup(elem);
		if (dup_elem)
			memcpy(((unsigned char *)out_buff) + (i * sizeof(void *)),
			       &dup_elem, sizeof(void *));
		else {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_ECB)
			return (memset(((unsigned char *)out_buff) + (i * sizeof(void *)),
			               0, sizeof(void *)),
			        false);
		}
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
bool rb_map(const t_cm_rb *rb, t_cm_duplicator dup, void **out_buff) {
	bool r;

	if (!rb || !dup || !out_buff) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return (false);
	}
	r = rb_nmap(rb, (size_t)-1, dup, out_buff);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
	return (r);
}
