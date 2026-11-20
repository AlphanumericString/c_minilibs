#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lruc_tests.h"

NO_COV void test_cm_lruci_keyinhash(void) {
	t_cm_lruc   c;
	size_t      returned_hash;
	const char *key1 = "big key";
	const char *key2 = "void key";

	lruc_init(&c, 3, NULL);
	lruc_put(&c, key1, strlen(key1), "door");
	assert(lruci_keyinhash(&c, key1, strlen(key1), NULL) != NULL);
	assert(lruci_keyinhash(&c, key1, strlen(key1), &returned_hash) != NULL);
	assert(returned_hash == lruci_hash_djb2(key1, strlen(key1)) % c.buckets_nb);
	assert(lruci_keyinhash(&c, key2, strlen(key2), &returned_hash) == NULL);
	assert(returned_hash == lruci_hash_djb2(key2, strlen(key2)) % c.buckets_nb);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruci_keyinlist(void) {
	t_cm_lruc   c;
	const char *key1 = "big key";
	const char *key2 = "void key";

	lruc_init(&c, 3, NULL);
	lruc_put(&c, key1, strlen(key1), "door");
	assert(lruci_keyinlist(&c, key1, strlen(key1)) == c.MRU);
	assert(lruci_keyinlist(&c, key2, strlen(key2)) == NULL);
	assert(lruci_keyinlist(&c, "big kez", strlen(key1)) == NULL); // same len
	assert(lruci_keyinlist(&c, key1, strlen(key1) - 1) == NULL);  // diff len
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruci_node_eq(void) {
	t_cm_lruc_keynode kv1 = {0}, kv2 = {0};

	assert(lruci_node_eq(&kv1, &kv1) == true);
	kv1.kv.key   = "aaaa";
	kv1.kv.k_len = 4;
	kv2.kv       = kv1.kv;
	assert(lruci_node_eq(&kv1, &kv2) == true);
	kv2.kv.key = "bbbb";
	assert(lruci_node_eq(&kv1, &kv2) == false);
	assert(lruci_key_eq("a", 1, "a", 1) == true);
	assert(lruci_key_eq("a", 1, "b", 1) == false);
	assert(lruci_key_eq("a", 1, "a", 2) == false);
	return;
}
NO_COV void test_cm_lruci_unlink_node(void) {
	t_cm_lruc          c;
	t_cm_lruc_keynode *node;

	lruc_init(&c, 3, NULL);
	lruc_put(&c, "a", 1, "A");
	lruc_put(&c, "b", 1, "B");
	node = c.LRU;
	lruci_unlink_node(&c, node);
	assert(c.node_nb == 1);
	assert(lruci_rsrvd_push(&c, node) == true); // detached node, hand it back
	assert(lruc_has(&c, "a", 1) == false);
	assert(lruc_has(&c, "b", 1) == true);
	assert(lruci_validate(&c) == true);
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruci_validate(void) {
	t_cm_lruc          c, intact;
	t_cm_lruc_keynode  saved, middle_saved, ghost = {0};
	t_cm_lruc_keynode *node, *middle, *saved_head, *saved_next;
	size_t             h;

	lruc_init(&c, 3, NULL);
	assert(lruci_validate(&c) == true); // empty cache
	lruc_put(&c, "a", 1, "A");
	lruc_put(&c, "b", 1, "B");
	lruc_put(&c, "c", 1, "C");
	assert(lruci_validate(&c) == true);
	intact       = c;
	node         = c.MRU;
	middle       = c.MRU->lru_next;
	saved        = *node;
	middle_saved = *middle;

	// a count smaller than the recency list
	c.node_nb = intact.node_nb - 1;
	assert(lruci_validate(&c) == false);
	c = intact;

	// a head that claims a predecessor
	node->lru_prev = node;
	assert(lruci_validate(&c) == false);
	*node = saved;

	// a back link that does not match its predecessor
	middle->lru_prev = middle;
	assert(lruci_validate(&c) == false);
	*middle = middle_saved;

	// a list shorter than node_nb
	c.LRU->lru_prev->lru_next = NULL;
	assert(lruci_validate(&c) == false);
	c.LRU->lru_prev->lru_next = c.LRU;

	// LRU that is not the list tail
	c.LRU = middle;
	assert(lruci_validate(&c) == false);
	c = intact;

	// a recency list cycle: the bounded walk must not hang
	node->lru_prev  = c.LRU;
	c.LRU->lru_next = node;
	assert(lruci_validate(&c) == false);
	*node           = saved;
	c.LRU->lru_next = NULL;
	assert(lruci_validate(&c) == true);

	// a node in the list but not in its bucket
	h            = lruci_hash_djb2(node->kv.key, node->kv.k_len) % c.buckets_nb;
	saved_head   = c.buckets[h];
	saved_next   = node->hash_next;
	c.buckets[h] = saved_next;
	assert(lruci_validate(&c) == false);
	c.buckets[h]    = saved_head;
	node->hash_next = saved_next;

	// a bucket cycle
	assert(c.buckets[h] == node);
	node->hash_next = node;
	assert(lruci_validate(&c) == false);
	node->hash_next = saved_next;

	// a bucket node missing from the list
	c.buckets[h] = NULL;
	assert(lruci_validate(&c) == false);
	c.buckets[h] = saved_head;
	assert(lruci_validate(&c) == true);

	// a reserved node carrying a live link
	lruc_remove(&c, "a", 1);
	assert(lruci_validate(&c) == true);
	assert(c.reserved_nodes != NULL);
	c.reserved_nodes->hash_next = &ghost;
	assert(lruci_validate(&c) == false);
	c.reserved_nodes->hash_next = NULL;
	c.reserved_nodes->lru_prev  = &ghost;
	assert(lruci_validate(&c) == false);
	c.reserved_nodes->lru_prev = NULL;
	assert(lruci_validate(&c) == true);

	assert(lruci_validate(NULL) == false);
	lruc_destroy(&c);
	return;
}
