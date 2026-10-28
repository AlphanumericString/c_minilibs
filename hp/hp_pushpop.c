#include "heap.h"

static void heap_swap(t_hp *h, size_t pos1, size_t pos2) {
	void *e = h->elems[pos1];

	h->elems[pos1] = h->elems[pos2];
	h->elems[pos2] = e;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SWAP, pos1, pos2)
}
static void heap_up(t_hp *h, size_t idx, bool max) {
	size_t parent = idx / 2;
	int    cmp_res;

	while (idx) {
		parent  = (idx - 1) / 2;
		cmp_res = h->cmp(h->elems[parent], h->elems[idx]);
		if ((max && cmp_res >= 0) || (!max && cmp_res <= 0)) break;
		heap_swap(h, parent, idx);
		idx = parent;
	}
}
// idx: index from witch to heap down
// h: heap to heap down
// max: is max heap
static void heap_down(t_hp *h, size_t idx, bool max) {
	int    cmp_res;
	size_t l, r;
	size_t target;

	while (1) {
		l      = idx * 2 + 1;
		r      = idx * 2 + 2;
		target = idx;
		if (l < h->nb) {
			cmp_res = h->cmp(h->elems[l], h->elems[target]);
			if ((max && cmp_res > 0) || (!max && cmp_res < 0)) target = l;
		}
		if (r < h->nb) {
			cmp_res = h->cmp(h->elems[r], h->elems[target]);
			if ((max && cmp_res > 0) || (!max && cmp_res < 0)) target = r;
		}
		if (target == idx) break;
		heap_swap(h, idx, target);
		idx = target;
	}
}

/**
 * \brief Push an element into the heap
 * \param h heap pointer
 * \param e element to push
 * \ingroup hp
**/
void hp_min_push(t_hp *h, void *e) {
	if (!h || !e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return;
	}
	if (h->nb >= h->capacity) return;
	h->elems[h->nb++] = e;
	heap_up(h, h->nb - 1, false);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_PUSH, e, (void *)h, h->nb,
	       h->capacity)
}
/**
 * \brief Push an element into the heap
 * \param h heap pointer
 * \param e element to push
 * \ingroup hp
**/
void hp_max_push(t_hp *h, void *e) {
	if (!h || !e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return;
	}
	if (h->nb >= h->capacity) return;
	h->elems[h->nb++] = e;
	heap_up(h, h->nb - 1, true);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_PUSH, e, (void *)h, h->nb,
	       h->capacity)
}

/**
 * \brief Pop the minimum element from the heap
 * \param h heap pointer
 * \return minimum element
 * \ingroup hp
**/
void *hp_min_pop(t_hp *h) {
	void *root;

	if (!h || !h->nb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	root        = h->elems[0];
	h->elems[0] = h->elems[h->nb - 1];
	h->nb--;
	heap_down(h, 0, false);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_POP, root, (void *)h, h->nb,
	       h->capacity)
	return (root);
}
/**
 * \brief Pop the maximum element from the heap
 * \param h heap pointer
 * \return maximum element
 * \ingroup hp
**/
void *hp_max_pop(t_hp *h) {
	void *root;

	if (!h || !h->nb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	root = h->elems[0];
	heap_swap(h, 0, h->nb - 1);
	h->nb--;
	heap_down(h, 0, true);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_POP, root, (void *)h, h->nb,
	       h->capacity)
	return (root);
}

/**
 * \brief Remove an element from the heap
 * \param h heap pointer
 * \param n index of the element to remove
 * \return element removed
 * \ingroup hp
**/
void *hp_min_rm(t_hp *h, size_t n) {
	if (!h) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	if (n > h->nb) n = h->nb - 1;
	if (!n) return (hp_min_pop(h));
	heap_swap(h, 0, n);
	heap_down(h, n, false);
	return (hp_min_pop(h));
}
/**
 * \brief Remove an element from the heap
 * \param h heap pointer
 * \param n index of the element to remove
 * \return element removed
 * \ingroup hp
**/
void *hp_max_rm(t_hp *h, size_t n) {
	if (!h) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	if (n > h->nb) n = h->nb - 1;
	if (!n) return (hp_max_pop(h));
	heap_swap(h, 0, n);
	heap_down(h, n, true);
	return (hp_max_pop(h));
}
