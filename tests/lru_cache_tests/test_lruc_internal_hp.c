#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lru_cache_tests.h"

void test_lruci_hp_pop(void) {
	t_lruc          c, intact_copy;
	t_lruc_keynode *kv1, *kv2;
	void           *key  = "quatorze";
	void           *key2 = "quinze";

	lruc_init(&c, 2, free);
	lruc_put(&c, key, strlen(key), malloc(14));
	kv1 = c.MRU;
	lruc_put(&c, key2, strlen(key2), malloc(15));
	kv2 = c.MRU;
	assert(kv2->kv.key == key2);

	assert(c.minHeap != NULL);
	assert(c.minHeap[0] != NULL && c.minHeap[0] == kv1);
	assert(c.minHeap[1] != NULL && c.minHeap[1] == kv2);
	assert(c.node_nb == 2);

	assert(lruci_hp_pop(&c) == kv1);
	assert(c.node_nb == 1);
	assert(c.minHeap[0] != NULL && c.minHeap[0] == kv2);
	// assert(c.minHeap[1] == NULL);  // minheap may not be null'ed
	intact_copy = c;

	c.minHeap = NULL;
	assert(lruci_hp_pop(&c) == NULL);
	c = intact_copy;

	c.node_nb = 0;
	assert(lruci_hp_pop(&c) == NULL);
	c = intact_copy;

	free(kv1->kv.value), free(kv2->kv.value);
	free(kv1), free(kv2);
	free(c.buckets);
}
void test_lruci_hp_pop_nth(void) {
	t_lruc          c, intact_copy;
	t_lruc_keynode *kv1, *kv2, *kv3;
	void           *key1 = "quatorze";
	void           *key2 = "quinze";
	void           *key3 = "quaize";

	lruc_init(&c, 3, free);
	lruc_put_ttl(&c, key1, strlen(key1), malloc(14), 100);
	kv1 = c.MRU;
	lruc_put_ttl(&c, key2, strlen(key2), malloc(15), 200);
	kv2 = c.MRU;
	lruc_put_ttl(&c, key3, strlen(key3), malloc(16), 300);
	kv3 = c.MRU;
	assert(kv1 != NULL), assert(kv1->kv.key == key1);
	assert(kv2 != NULL), assert(kv2->kv.key == key2);
	assert(kv3 != NULL), assert(kv3->kv.key == key3);

	assert(c.minHeap != NULL);
	assert(c.minHeap[0] != NULL), assert(c.minHeap[0] == kv1);
	assert(c.minHeap[1] != NULL), assert(c.minHeap[1] == kv2);
	assert(c.minHeap[2] != NULL), assert(c.minHeap[2] == kv3);
	assert(c.node_nb == 3);

	assert(lruci_hp_pop_nth(&c, 1) == kv2);
	assert(c.node_nb == 2);
	assert(c.minHeap[0] != NULL), assert(c.minHeap[0] == kv1);
	assert(c.minHeap[1] != NULL), assert(c.minHeap[1] == kv3);
	assert(c.node_nb == 2);
	assert(lruci_hp_pop_nth(&c, -1) == kv3);
	assert(c.node_nb == 1);
	assert(c.minHeap[0] != NULL), assert(c.minHeap[0] == kv1);
	intact_copy = c;

	c.minHeap = NULL;
	assert(lruci_hp_pop_nth(&c, -1) == NULL);
	c = intact_copy;

	c.node_nb = 0;
	assert(lruci_hp_pop_nth(&c, -1) == NULL);
	c = intact_copy;

	free(kv1->kv.value), free(kv2->kv.value), free(kv3->kv.value);
	free(kv1), free(kv2), free(kv3);
	free(c.buckets);
}
