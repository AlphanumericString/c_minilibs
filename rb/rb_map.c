#include <string.h>

#include "round_buffer.h"

/**
 * \brief apply duplicator to up to n elements and store in out_buff
 * \param rb the round buffer
 * \param n maximum number of elements to map
 * \param dup the duplicator function
 * \param out_buff array where the adress of the duplicated elements are copied
 * \return true on success, false if an element couldn't be duplicated
**/
bool rb_nmap(t_rb *rb, size_t n, t_duplicator dup, void **out_buff) {
	size_t i = 0;
	size_t pos;
	size_t count;
	void  *elem;
	void  *dup_elem;

	if (!rb || !dup || !out_buff || !n) return (false);

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
			return (bzero(((unsigned char *)out_buff) + (i * sizeof(void *)),
			              sizeof(void *)),
			        false);
		}
		pos = (pos + 1) % rb->nb_max;
		i++;
	}
	return (true);
}

/**
 * \brief apply duplicator to all elements and store in out_buff
 * \param rb the round buffer
 * \param dup the duplicator function
 * \param out_buff array where the adress of the duplicated elements are copied
 * \return true on success, false if an element couldn't be duplicated
**/
bool rb_map(t_rb *rb, t_duplicator dup, void **out_buff) {
	if (!rb || !dup || !out_buff) return (false);
	return (rb_nmap(rb, (size_t)-1, dup, out_buff));
}
