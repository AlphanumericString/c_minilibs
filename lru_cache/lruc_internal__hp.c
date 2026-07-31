#define C_MINI__LRUC_INTERNAL
#include "least_recently_used_cache.h"

typedef bool (*Comparator)(t_lruc_keynode a, t_lruc_keynode b);

static inline void lruci_heap_swap(t_lruc *c, size_t a, size_t b) {
	t_lruc_keynode *temp = c->minHeap[a];

	c->minHeap[a]             = c->minHeap[b];
	c->minHeap[b]             = temp;
	c->minHeap[a]->heap_index = a;
	c->minHeap[b]->heap_index = b;
}

static inline bool compare(const t_lruc_keynode *a, const t_lruc_keynode *b) {
	return (a->time < b->time);
}

static void lruci_hp_up(t_lruc *c, size_t index) {
	size_t parent;

	while (index > 0) {
		parent = (index - 1) / 2;
		if (compare(c->minHeap[index], c->minHeap[parent])) {
			lruci_heap_swap(c, index, parent);
			index = parent;
		} else {
			break;
		}
	}
}
static void lruci_hp_down(t_lruc *c, size_t index) {
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
			lruci_heap_swap(c, index, target);
			index = target;
		} else {
			break;
		}
	}
}

void lruci_hp_push(t_lruc *c, t_lruc_keynode *value) {
	c->minHeap[c->node_nb]             = value;
	c->minHeap[c->node_nb]->heap_index = c->node_nb;
	lruci_hp_up(c, c->node_nb);
	c->node_nb++;
}

t_lruc_keynode *lruci_hp_pop(t_lruc *c) {
	t_lruc_keynode *root_value = NULL;

	if (!c->node_nb || !c->minHeap) return NULL;
	root_value                = c->minHeap[0];
	c->minHeap[0]             = c->minHeap[c->node_nb - 1];
	c->minHeap[0]->heap_index = 0;
	c->node_nb--;
	if (!c->node_nb) c->minHeap[0] = NULL;
	else lruci_hp_down(c, 0);
	root_value->heap_index = -1;
	return (root_value);
}
t_lruc_keynode *lruci_hp_pop_nth(t_lruc *c, size_t n) {
	if (!c->node_nb || !c->minHeap) return NULL;
	if (n > (c->node_nb - 1)) n = c->node_nb - 1;
	lruci_heap_swap(c, 0, n);
	return (lruci_hp_pop(c));
}
