#include <string.h>

#include "hp_internal.h"

/**
 * \brief filter elements of a min heap keeping those that match the condition
 * \param h heap pointer
 * \param keep callback to test elements, true:keep false:out
 * \param arg user data for callbacks
 * \param del optional callback to delete removed elements
 * \return true on success, false otherwise
 * \note the heap invariant is rebuilt after compaction, the relative order of
 * kept elements is not preserved
 * \ingroup hp
**/
bool hp_min_filter(t_cm_hp *h, t_cm_callback keep, void *arg,
                   t_cm_callback del) {
	void  *elem;
	size_t write;

	if (!hp_is_valid(h) || !keep || hp_is_empty(h)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	write = 0;
	for (size_t read = 0; read < h->nb; read++) {
		elem = hpi_elem_ptr(h, read);
		if (keep(elem, arg)) {
			if (write != read) memmove(hpi_elem_ptr(h, write), elem, h->sz_e);
			write++;
		} else if (del) {
			del(elem, arg);
		}
	}
	h->nb = write;
	hpi_heapify(h, false);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}

/**
 * \brief filter elements of a max heap keeping those that match the condition
 * \param h heap pointer
 * \param keep callback to test elements, true:keep false:out
 * \param arg user data for callbacks
 * \param del optional callback to delete removed elements
 * \return true on success, false otherwise
 * \note the heap invariant is rebuilt after compaction, the relative order of
 * kept elements is not preserved
 * \ingroup hp
**/
bool hp_max_filter(t_cm_hp *h, t_cm_callback keep, void *arg,
                   t_cm_callback del) {
	void  *elem;
	size_t write;

	if (!hp_is_valid(h) || !keep || hp_is_empty(h)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	write = 0;
	for (size_t read = 0; read < h->nb; read++) {
		elem = hpi_elem_ptr(h, read);
		if (keep(elem, arg)) {
			if (write != read) memmove(hpi_elem_ptr(h, write), elem, h->sz_e);
			write++;
		} else if (del) {
			del(elem, arg);
		}
	}
	h->nb = write;
	hpi_heapify(h, true);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}
