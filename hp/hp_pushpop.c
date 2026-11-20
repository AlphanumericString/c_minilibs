#define C_MINI__HP_INTERNAL

#include <string.h>

#include "heap.h"

/**
 * \brief Push an element into the heap
 * \param h heap pointer
 * \param e element to push
 * \return true on success, false otherwise
 * \ingroup hp
**/
bool hp_min_push(t_cm_hp *h, const void *e) {
	if (!h || !e || !h->sz_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	if (h->nb >= h->capacity) return (false);
	memcpy(hpi_elem_ptr(h, h->nb), e, h->sz_e);
	h->nb++;
	hpi_sift_up(h, h->nb - 1, false);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_PUSH, e, (void *)h, h->nb,
	       h->capacity, h->sz_e)
	return (true);
}
/**
 * \brief Push an element into the heap
 * \param h heap pointer
 * \param e element to push
 * \return true on success, false otherwise
 * \ingroup hp
**/
bool hp_max_push(t_cm_hp *h, const void *e) {
	if (!h || !e || !h->sz_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	if (h->nb >= h->capacity) return (false);
	memcpy(hpi_elem_ptr(h, h->nb), e, h->sz_e);
	h->nb++;
	hpi_sift_up(h, h->nb - 1, true);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_PUSH, e, (void *)h, h->nb,
	       h->capacity, h->sz_e)
	return (true);
}

/**
 * \brief Pop the minimum element from the heap and copy it at out
 * \param h heap pointer
 * \param out buffer where the popped element is copied
 * \return true on success, false otherwise
 * \ingroup hp
**/
bool hp_min_pop(t_cm_hp *h, void *out) {
	if (!h || !out || !h->nb || !h->sz_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	memcpy(out, hpi_elem_ptr(h, 0), h->sz_e);
	h->nb--;
	if (h->nb) memcpy(hpi_elem_ptr(h, 0), hpi_elem_ptr(h, h->nb), h->sz_e);
	hpi_sift_down(h, 0, false);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_POP, out, (void *)h, h->nb,
	       h->capacity, h->sz_e)
	return (true);
}
/**
 * \brief Pop the maximum element from the heap and copy it at out
 * \param h heap pointer
 * \param out buffer where the popped element is copied
 * \return true on success, false otherwise
 * \ingroup hp
**/
bool hp_max_pop(t_cm_hp *h, void *out) {
	if (!h || !out || !h->nb || !h->sz_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	memcpy(out, hpi_elem_ptr(h, 0), h->sz_e);
	h->nb--;
	if (h->nb) memcpy(hpi_elem_ptr(h, 0), hpi_elem_ptr(h, h->nb), h->sz_e);
	hpi_sift_down(h, 0, true);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_POP, out, (void *)h, h->nb,
	       h->capacity, h->sz_e)
	return (true);
}

/**
 * \brief Remove an element from the heap and copy it at out
 * \param h heap pointer
 * \param n index of the element to remove
 * \param out buffer where the removed element is copied
 * \return true on success, false otherwise
 * \ingroup hp
**/
bool hp_min_rm(t_cm_hp *h, size_t n, void *out) {
	if (!h || !out || !h->nb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	if (n >= h->nb) n = h->nb - 1;
	if (!n) return (hp_min_pop(h, out));
	hpi_swap(h, 0, n);
	return (hp_min_pop(h, out));
}
/**
 * \brief Remove an element from the heap and copy it at out
 * \param h heap pointer
 * \param n index of the element to remove
 * \param out buffer where the removed element is copied
 * \return true on success, false otherwise
 * \ingroup hp
**/
bool hp_max_rm(t_cm_hp *h, size_t n, void *out) {
	if (!h || !out || !h->nb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	if (n >= h->nb) n = h->nb - 1;
	if (!n) return (hp_max_pop(h, out));
	hpi_swap(h, 0, n);
	return (hp_max_pop(h, out));
}

/**
 * \brief Push then pop in one operation, keeping the heap size unchanged.
 * The smallest of the root and e is copied at out.
 * \param h heap pointer
 * \param e element to push before popping
 * \param out buffer where the popped element is copied
 * \return true on success, false otherwise
 * \note on an empty heap e is copied at out and the heap stays empty
 * \ingroup hp
**/
bool hp_min_pushpop(t_cm_hp *h, const void *e, void *out) {
	if (!h || !e || !out || !h->sz_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	if (!h->nb || h->cmp(e, hpi_elem_ptr(h, 0)) <= 0) {
		memcpy(out, e, h->sz_e);
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
		return (true);
	}
	hpi_replace_root(h, e, out, false);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}
/**
 * \brief Push then pop in one operation, keeping the heap size unchanged.
 * The largest of the root and e is copied at out.
 * \param h heap pointer
 * \param e element to push before popping
 * \param out buffer where the popped element is copied
 * \return true on success, false otherwise
 * \note on an empty heap e is copied at out and the heap stays empty
 * \ingroup hp
**/
bool hp_max_pushpop(t_cm_hp *h, const void *e, void *out) {
	if (!h || !e || !out || !h->sz_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	if (!h->nb || h->cmp(e, hpi_elem_ptr(h, 0)) >= 0) {
		memcpy(out, e, h->sz_e);
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
		return (true);
	}
	hpi_replace_root(h, e, out, true);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}

/**
 * \brief Pop the root and push e in one operation, keeping the heap size
 * unchanged. The previous root is copied at out.
 * \param h heap pointer
 * \param e element inserted after the pop
 * \param out buffer where the popped element is copied
 * \return true on success, false otherwise
 * \note fails on an empty heap
 * \ingroup hp
**/
bool hp_min_replace(t_cm_hp *h, const void *e, void *out) {
	if (!h || !e || !out || !h->nb || !h->sz_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	hpi_replace_root(h, e, out, false);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}
/**
 * \brief Pop the root and push e in one operation, keeping the heap size
 * unchanged. The previous root is copied at out.
 * \param h heap pointer
 * \param e element inserted after the pop
 * \param out buffer where the popped element is copied
 * \return true on success, false otherwise
 * \note fails on an empty heap
 * \ingroup hp
**/
bool hp_max_replace(t_cm_hp *h, const void *e, void *out) {
	if (!h || !e || !out || !h->nb || !h->sz_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	hpi_replace_root(h, e, out, true);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}
