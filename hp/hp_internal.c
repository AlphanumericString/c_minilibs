#include "hp_internal.h"

#include <string.h>

void *hpi_elem_ptr(const t_cm_hp *h, size_t idx) {
	return ((char *)h->elems + h->sz_e * idx);
}

static void hpi_swap_bytes(void *a, void *b, size_t n) {
	unsigned char *sa = (unsigned char *)a;
	unsigned char *sb = (unsigned char *)b;
	unsigned char  tmp;

	while (n--) {
		tmp   = sa[n];
		sa[n] = sb[n];
		sb[n] = tmp;
	}
}

void hpi_swap(const t_cm_hp *h, size_t pos1, size_t pos2) {
	hpi_swap_bytes(hpi_elem_ptr(h, pos1), hpi_elem_ptr(h, pos2), h->sz_e);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SWAP, pos1, pos2)
}

void hpi_sift_up(t_cm_hp *h, size_t idx, bool max) {
	size_t parent;
	int    cmp_res;

	while (idx) {
		parent  = (idx - 1) / 2;
		cmp_res = h->cmp(hpi_elem_ptr(h, parent), hpi_elem_ptr(h, idx));
		if ((max && cmp_res >= 0) || (!max && cmp_res <= 0)) break;
		hpi_swap(h, parent, idx);
		idx = parent;
	}
}

void hpi_sift_down(t_cm_hp *h, size_t idx, bool max) {
	int    cmp_res;
	size_t l;
	size_t r;
	size_t target;

	for (;; idx = target) {
		l      = idx * 2 + 1;
		r      = idx * 2 + 2;
		target = idx;
		if (l < h->nb) {
			cmp_res = h->cmp(hpi_elem_ptr(h, l), hpi_elem_ptr(h, target));
			if ((max && cmp_res > 0) || (!max && cmp_res < 0)) target = l;
		}
		if (r < h->nb) {
			cmp_res = h->cmp(hpi_elem_ptr(h, r), hpi_elem_ptr(h, target));
			if ((max && cmp_res > 0) || (!max && cmp_res < 0)) target = r;
		}
		if (target == idx) break;
		hpi_swap(h, idx, target);
	}
}

void hpi_heapify(t_cm_hp *h, bool max) {
	size_t i;

	if (h->nb < 2) return;
	for (i = h->nb / 2; i > 0; i--)
		hpi_sift_down(h, i - 1, max);
}

void hpi_replace_root(t_cm_hp *h, const void *e, void *out, bool max) {
	memcpy(out, hpi_elem_ptr(h, 0), h->sz_e);
	memcpy(hpi_elem_ptr(h, 0), e, h->sz_e);
	hpi_sift_down(h, 0, max);
}
