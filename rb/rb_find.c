#define C_MINI__RB_INTERNAL
#include "round_buffer.h"

/**
 * \brief finds the first element that matches the comparator
 * \param rb the round buffer
 * \param c the comparator function
 * \param a argument for the comparator (nullable)
 * \return pointer to the element in the data array, or NULL if not found
 * \ingroup rb
**/
void *rb_find(const t_rb *rb, t_cm_comparator c, const void *a) {
	size_t i = 0;
	size_t pos;
	size_t count;
	void  *elem;

	if (!rb || !c) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RB_LOG_EARG)
		return false;
	}
	count = rb_size(*rb);
	pos   = rbi_opos_get(rb);
	while (i < count) {
		elem = rb->array + (pos * rb->sz_e);
		if (c(elem, (void *)a) == 0) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_SUCCESS)
			return elem;
		}
		pos = (pos + 1) % rb->nb_max;
		i++;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RB_LOG_FAIL)
	return NULL;
}
