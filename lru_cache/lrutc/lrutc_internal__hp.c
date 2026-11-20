#define C_MINI__LRUTC_INTERNAL
#include "least_recently_used_timed_cache.h"

typedef bool (*Comparator)(t_cm_lrutc_keynode a, t_cm_lrutc_keynode b);

static inline void lrutci_heap_swap(t_cm_lrutc *c, size_t a, size_t b) {
	t_cm_lrutc_keynode *temp = c->minHeap[a];

	c->minHeap[a]             = c->minHeap[b];
	c->minHeap[b]             = temp;
	c->minHeap[a]->heap_index = a;
	c->minHeap[b]->heap_index = b;
}

static inline bool compare(const t_cm_lrutc_keynode *a,
                           const t_cm_lrutc_keynode *b) {
	return (lrutci_expiry(a) < lrutci_expiry(b));
}

static void lrutci_hp_up(t_cm_lrutc *c, size_t index) {
	size_t parent;

	while (index > 0) {
		parent = (index - 1) / 2;
		if (compare(c->minHeap[index], c->minHeap[parent])) {
			lrutci_heap_swap(c, index, parent);
			index = parent;
		} else {
			break;
		}
	}
}
static void lrutci_hp_down(t_cm_lrutc *c, size_t index) {
	size_t L;
	size_t R;
	size_t target;

	while (1) {
		L      = 2 * index + 1;
		R      = 2 * index + 2;
		target = index;

		if (L < c->node_nb && compare(c->minHeap[L], c->minHeap[target]))
			target = L;
		if (R < c->node_nb && compare(c->minHeap[R], c->minHeap[target]))
			target = R;

		if (target != index) {
			lrutci_heap_swap(c, index, target);
			index = target;
		} else {
			break;
		}
	}
}

void lrutci_hp_push(t_cm_lrutc *c, t_cm_lrutc_keynode *value) {
	c->minHeap[c->node_nb]             = value;
	c->minHeap[c->node_nb]->heap_index = c->node_nb;
	lrutci_hp_up(c, c->node_nb);
	c->node_nb++;
}

t_cm_lrutc_keynode *lrutci_hp_pop(t_cm_lrutc *c) {
	t_cm_lrutc_keynode *root_value = NULL;

	if (!c->node_nb || !c->minHeap) return NULL;
	root_value                = c->minHeap[0];
	c->minHeap[0]             = c->minHeap[c->node_nb - 1];
	c->minHeap[0]->heap_index = 0;
	c->node_nb--;
	if (!c->node_nb) c->minHeap[0] = NULL;
	else lrutci_hp_down(c, 0);
	root_value->heap_index = C_MINI__LRUTC_NO_HEAP_SLOT; // no longer tracked
	return (root_value);
}
t_cm_lrutc_keynode *lrutci_hp_pop_nth(t_cm_lrutc *c, size_t n) {
	if (!c->node_nb || !c->minHeap) return NULL;
	if (n > (c->node_nb - 1)) n = c->node_nb - 1;
	lrutci_heap_swap(c, 0, n);
	return (lrutci_hp_pop(c));
}

void lrutci_hp_rebuild(t_cm_lrutc *c) {
	size_t             i = 0;
	t_cm_lrutc_keynode *node;

	if (!c->minHeap) return;
	for (node = c->MRU; node; node = node->lru_next) {
		c->minHeap[i]    = node;
		node->heap_index = i;
		i++;
	}
	if (i < c->buckets_nb) c->minHeap[i] = NULL;
	for (i = c->node_nb / 2; i-- > 0;)
		lrutci_hp_down(c, i);
}
