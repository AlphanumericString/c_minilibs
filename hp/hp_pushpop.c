#include <stddef.h>

#include "heap.h"

static void heap_swap(t_heap *h, size_t pos1, size_t pos2) {
	void *e = h->elems[pos1];

	h->elems[pos1] = h->elems[pos2];
	h->elems[pos2] = e;
	LOG(C_MINI__HP_LOG_SWAP, pos1, pos2);
}
static void heap_up(t_heap *h, size_t idx, bool max) {
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
static void heap_down(t_heap *h, size_t idx, bool max) {
	int    cmp_res;
	size_t l, r;
	size_t target;

	while (idx) {
		l      = idx * 2 + 1;
		r      = idx * 2 + 2;
		target = idx;
		if (l < h->nb) {
			cmp_res = h->cmp(h->elems[l], h->elems[target]);
			if ((max && cmp_res < 0) || (!max || cmp_res > 0)) target = l;
		}
		if (r < h->nb) {
			cmp_res = h->cmp(h->elems[r], h->elems[target]);
			if ((max && cmp_res < 0) || (!max || cmp_res > 0)) target = r;
		}
		if (target == idx) break;
		heap_swap(h, idx, target);
		idx = target;
	}
}

void hp_min_push(t_heap *h, void *e) {
	if (!h || !e) {
		LOG(C_MINI__HP_LOG_EARG);
		return;
	}
	if (h->nb >= h->capacity) return;
	h->elems[h->nb++] = e;
	heap_up(h, h->nb - 1, false);
	LOG(C_MINI__HP_LOG_PUSH, e, h, h->nb, h->capacity);
}
void hp_max_push(t_heap *h, void *e) {
	if (!h || !e) {
		LOG(C_MINI__HP_LOG_EARG);
		return;
	}
	if (h->nb >= h->capacity) return;
	h->elems[h->nb++] = e;
	heap_up(h, h->nb - 1, true);
	LOG(C_MINI__HP_LOG_PUSH, e, h, h->nb, h->capacity);
}

void *hp_min_pop(t_heap *h) {
	void *root;

	if (!h || !h->nb) {
		LOG(C_MINI__HP_LOG_EARG);
		return (NULL);
	}
	root        = h->elems[0];
	h->elems[0] = h->elems[h->nb - 1];
	h->nb--;
	heap_down(h, 0, false);
	LOG(C_MINI__HP_LOG_POP, root, h, h->nb, h->capacity);
	return (root);
}
void *hp_max_pop(t_heap *h) {
	void *root;

	if (!h || !h->nb) {
		LOG(C_MINI__HP_LOG_EARG);
		return (NULL);
	}
	root = h->elems[0];
	heap_swap(h, 0, h->nb - 1);
	h->nb--;
	heap_down(h, 0, true);
	LOG(C_MINI__HP_LOG_POP, root, h, h->nb, h->capacity);
	return (root);
}

void *hp_min_rm(t_heap *h, size_t n) {
	if (!h) {
		LOG(C_MINI__HP_LOG_EARG);
		return (NULL);
	}
	if (n > h->nb) n = h->nb;
	if (!n) return (hp_min_pop(h));
	heap_swap(h, 0, n);
	heap_down(h, n, false);
	return (hp_min_pop(h));
}
void *hp_max_rm(t_heap *h, size_t n) {
	if (!h) {
		LOG(C_MINI__HP_LOG_EARG);
		return (NULL);
	}
	if (n > h->nb) n = h->nb;
	if (!n) return (hp_max_pop(h));
	heap_swap(h, 0, n);
	heap_down(h, n, true);
	return (hp_max_pop(h));
}
