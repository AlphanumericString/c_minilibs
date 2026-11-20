#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lruc_tests.h"

NO_COV void test_cm_lruci_lru_push_pop(void) {
	t_cm_lruc          c = {0};
	t_cm_lruc_keynode  n1 = {0}, n2 = {0}, n3 = {0};

	lruci_lru_push(&c, &n1);
	assert(c.MRU == &n1 && c.LRU == &n1);
	assert(n1.lru_prev == NULL && n1.lru_next == NULL);
	lruci_lru_push(&c, &n2);
	assert(c.MRU == &n2 && c.LRU == &n1);
	assert(n2.lru_next == &n1 && n1.lru_prev == &n2);
	lruci_lru_push(&c, &n3);
	assert(c.MRU == &n3 && c.LRU == &n1);

	assert(lruci_lru_pop(&c, &n2) == &n2);
	assert(c.MRU == &n3 && c.LRU == &n1);
	assert(n3.lru_next == &n1 && n1.lru_prev == &n3);
	assert(n2.lru_next == NULL && n2.lru_prev == NULL);

	assert(lruci_lru_pop(&c, &n3) == &n3);
	assert(c.MRU == &n1 && c.LRU == &n1 && n1.lru_prev == NULL);
	assert(lruci_lru_pop(&c, &n1) == &n1);
	assert(c.MRU == NULL && c.LRU == NULL);
	return;
}
NO_COV void test_cm_lruci_hsh_pop(void) {
	t_cm_lruc          c = {0};
	t_cm_lruc_keynode  n1 = {.kv = {.key = "a", .k_len = 1}};
	t_cm_lruc_keynode  n2 = {.kv = {.key = "b", .k_len = 1}};
	t_cm_lruc_keynode  n3 = {.kv = {.key = "c", .k_len = 1}};
	size_t             h1, h2;

	c.buckets_nb = 1;
	c.buckets    = calloc(1, sizeof(*c.buckets));
	c.buckets[0] = &n1;
	n1.hash_next = &n2;
	n2.hash_next = &n3;

	h1 = lruci_hash_djb2("b", 1);
	assert(lruci_hsh_pop(&c, &n2, h1) == &n2);
	assert(c.buckets[0] == &n1 && n1.hash_next == &n3);
	assert(n2.hash_next == NULL);

	h2 = lruci_hash_djb2("a", 1);
	assert(lruci_hsh_pop(&c, &n1, h2) == &n1);
	assert(c.buckets[0] == &n3);
	assert(n1.hash_next == NULL);

	// absent node: no change
	assert(lruci_hsh_pop(&c, &n1, h2) == NULL);
	assert(c.buckets[0] == &n3);

	free(c.buckets);
	return;
}
NO_COV void test_cm_lruci_rsvrd_push_pop(void) {
	t_cm_lruc         c = {0};
	t_cm_lruc_keynode n1 = {.hash_next = (void *)1}, n2 = {0};
	t_cm_lruc_keynode *popped;

	assert(lruci_rsrvd_push(&c, &n1) == true);
	assert(c.reserved_nodes == &n1 && c.reserved_nodes_nb == 1);
	// pushing wipes the node, only the stack link survives
	assert(n1.hash_next == NULL && n1.lru_prev == NULL && n1.kv.key == NULL);
	assert(lruci_rsrvd_push(&c, &n2) == true);
	assert(c.reserved_nodes_nb == 2);
	assert(c.reserved_nodes == &n2 && n2.lru_next == &n1);

	popped = lruci_rsrvd_pop(&c);
	assert(popped == &n2 && c.reserved_nodes_nb == 1);
	popped = lruci_rsrvd_pop(&c);
	assert(popped == &n1 && c.reserved_nodes == NULL && c.reserved_nodes_nb == 0);
	assert(lruci_rsrvd_pop(&c) == NULL);
	return;
}
