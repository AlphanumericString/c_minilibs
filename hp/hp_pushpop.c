#include <string.h>

#include "heap.h"

static void *hp_elem_ptr(const t_cm_hp *h, size_t idx) {
	return ((char *)h->elems + h->sz_e * idx);
}

static void hp_swap_bytes(void *a, void *b, size_t n) {
	unsigned char *sa = (unsigned char *)a;
	unsigned char *sb = (unsigned char *)b;
	unsigned char  tmp;

	while (n--) {
		tmp   = sa[n];
		sa[n] = sb[n];
		sb[n] = tmp;
	}
}

static void heap_swap(const t_cm_hp *h, size_t pos1, size_t pos2) {
	hp_swap_bytes(hp_elem_ptr(h, pos1), hp_elem_ptr(h, pos2), h->sz_e);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SWAP, pos1, pos2)
}
// heapup in max or min mode, both works
static void heap_up(t_cm_hp *h, size_t idx, bool max) {
	size_t parent;

	for (int cmp_res; idx;) {
		parent  = (idx - 1) / 2;
		cmp_res = h->cmp(hp_elem_ptr(h, parent), hp_elem_ptr(h, idx));
		if ((max && cmp_res >= 0) || (!max && cmp_res <= 0)) break;
		heap_swap(h, parent, idx);
		idx = parent;
	}
}
// idx: index from witch to heap down
// h: heap to heap down
// max: is max heap
// heapdown in max or min mode, both works
static void heap_down(t_cm_hp *h, size_t idx, bool max) {
	int cmp_res;

	for (size_t l, r, target; 1; idx = target) {
		l      = idx * 2 + 1;
		r      = idx * 2 + 2;
		target = idx;
		if (l < h->nb) {
			cmp_res = h->cmp(hp_elem_ptr(h, l), hp_elem_ptr(h, target));
			if ((max && cmp_res > 0) || (!max && cmp_res < 0)) target = l;
		}
		if (r < h->nb) {
			cmp_res = h->cmp(hp_elem_ptr(h, r), hp_elem_ptr(h, target));
			if ((max && cmp_res > 0) || (!max && cmp_res < 0)) target = r;
		}
		if (target == idx) break;
		heap_swap(h, idx, target);
	}
}

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
	memcpy(hp_elem_ptr(h, h->nb), e, h->sz_e);
	h->nb++;
	heap_up(h, h->nb - 1, false);
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
	memcpy(hp_elem_ptr(h, h->nb), e, h->sz_e);
	h->nb++;
	heap_up(h, h->nb - 1, true);
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
	memcpy(out, hp_elem_ptr(h, 0), h->sz_e);
	h->nb--;
	if (h->nb) memcpy(hp_elem_ptr(h, 0), hp_elem_ptr(h, h->nb), h->sz_e);
	heap_down(h, 0, false);
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
	memcpy(out, hp_elem_ptr(h, 0), h->sz_e);
	h->nb--;
	if (h->nb) memcpy(hp_elem_ptr(h, 0), hp_elem_ptr(h, h->nb), h->sz_e);
	heap_down(h, 0, true);
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
	heap_swap(h, 0, n);
	heap_down(h, n, false);
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
	heap_swap(h, 0, n);
	heap_down(h, n, true);
	return (hp_max_pop(h, out));
}
