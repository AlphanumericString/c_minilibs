#include <assert.h>
#include <string.h>

#include "lrutc_tests.h"

NO_COV void test_cm_lrutci_keyinhash(void) {
	t_cm_lrutc   c;
	size_t      returnd_hash, actual_hash;
	const char *key1 = "big key";
	const char *key2 = "small key";
	const char *key3 = "skull key";
	const char *key4 = "void key";

	const char *val1 = "big door";
	const char *val2 = "small door";
	const char *val3 = "skull door";

	lrutc_init(&c, 3, NULL);
	lrutc_put(&c, key1, strlen(key1), val1);
	lrutc_put(&c, key2, strlen(key2), val2);
	lrutc_put(&c, key3, strlen(key3), val3);
	assert(lrutci_keyinhash(&c, key1, strlen(key1), NULL) != NULL);
	assert(lrutci_keyinhash(&c, key1, strlen(key1), &returnd_hash) != NULL);
	actual_hash = lrutci_hash_djb2(key1, strlen(key1));
	assert(returnd_hash == actual_hash % c.buckets_nb);
	assert(lrutci_keyinhash(&c, key2, strlen(key2), &returnd_hash) != NULL);
	actual_hash = lrutci_hash_djb2(key2, strlen(key2));
	assert(returnd_hash == actual_hash % c.buckets_nb);
	assert(lrutci_keyinhash(&c, key3, strlen(key3), &returnd_hash) != NULL);
	actual_hash = lrutci_hash_djb2(key3, strlen(key3));
	assert(returnd_hash == actual_hash % c.buckets_nb);
	assert(lrutci_keyinhash(&c, key4, strlen(key4), &returnd_hash) == NULL);
	actual_hash = lrutci_hash_djb2(key4, strlen(key4));
	assert(returnd_hash == actual_hash % c.buckets_nb);
	lrutc_destroy(&c);
}
NO_COV void test_cm_lrutci_keyinlist(void) {
	t_cm_lrutc   c;
	const char *key1 = "big key";
	const char *val1 = "big door";
	const char *key2 = "small key";
	const char *val2 = "small door";
	const char *key3 = "skull key";
	const char *val3 = "skull door";
	const char *key4 = "void key";

	lrutc_init(&c, 3, NULL);
	lrutc_put(&c, key1, strlen(key1), val1);
	lrutc_put(&c, key2, strlen(key2), val2);
	lrutc_put(&c, key3, strlen(key3), val3);
	assert(lrutci_keyinlist(&c, key1, strlen(key1)) != NULL);
	assert(lrutci_keyinlist(&c, key2, strlen(key2)) != NULL);
	assert(lrutci_keyinlist(&c, key3, strlen(key3)) != NULL);
	assert(lrutci_keyinlist(&c, key4, strlen(key4)) == NULL);
	lrutc_destroy(&c);
}
NO_COV void test_cm_lrutci_node_eq(void) {
	const char       *key1 = "aaaaAAAAAAAAAAAAAAAAa";
	const char       *key2 = "bbbbBBBBBBBBBBBBBbbbb";
	t_cm_lrutc_keynode kv1  = {0};
	t_cm_lrutc_keynode kv2  = kv1;

	assert(lrutci_node_eq(&kv1, &kv1) == true);
	assert(lrutci_node_eq(&kv2, &kv2) == true);
	kv1.kv.key   = key1;
	kv1.kv.k_len = 21;
	kv2.kv       = kv1.kv;
	assert(lrutci_node_eq(&kv1, &kv2) == true);
	kv1.kv.key = key2;
	assert(lrutci_node_eq(&kv1, &kv2) == false);
}
NO_COV void test_cm_lrutci_expiry(void) {
	t_cm_lrutc_keynode n = {.time_emitted = 0, .ns_gap = (size_t)-1};

	// no ttl -> infinite expiry
	assert(lrutci_expiry(&n) == (uint64_t)-1);

	// normal expiry is time_emitted + ns_gap
	n.time_emitted = 10;
	n.ns_gap       = 5;
	assert(lrutci_expiry(&n) == 15);

	// huge ns_gap overflows -> saturates to infinite
	n.time_emitted = 10;
	n.ns_gap       = (size_t)-2;
	assert(lrutci_expiry(&n) == (uint64_t)-1);
}
NO_COV void test_cm_lrutci_expired(void) {
	t_cm_lrutc_keynode n = {.time_emitted = 0, .ns_gap = (size_t)-1};

	// no ttl -> never expired
	assert(lrutci_expired(&n, 100) == false);

	n.time_emitted = 100;
	n.ns_gap       = 50;
	assert(lrutci_expired(&n, 100) == false); // now == time_emitted
	assert(lrutci_expired(&n, 50) == false);  // now < time_emitted
	assert(lrutci_expired(&n, 140) == false); // not expired yet
	assert(lrutci_expired(&n, 151) == true);  // expired
}

NO_COV void test_cm_lrutci_validate(void) {
	t_cm_lrutc          c, intact;
	t_cm_lrutc_keynode  saved, middle_saved, ghost = {0};
	t_cm_lrutc_keynode *node, *middle, *saved_head, *saved_next, *saved_slot0;
	size_t             h;

	ghost.heap_index = C_MINI__LRUTC_NO_HEAP_SLOT;
	lrutc_init(&c, 3, NULL);
	assert(lrutci_validate(&c) == true); // empty cache
	lrutc_put(&c, "a", 1, "A");
	lrutc_put(&c, "b", 1, "B");
	lrutc_put(&c, "c", 1, "C");
	assert(lrutci_validate(&c) == true);
	intact       = c;
	node         = c.MRU;
	middle       = c.MRU->lru_next;
	saved        = *node;
	middle_saved = *middle;
	saved_slot0  = c.minHeap[0];

	// a count smaller than the recency list
	c.node_nb = intact.node_nb - 1;
	assert(lrutci_validate(&c) == false);
	c = intact;

	// an empty heap slot
	c.minHeap[0] = NULL;
	assert(lrutci_validate(&c) == false);
	c            = intact;
	c.minHeap[0] = saved_slot0;

	// a node advertising a heap slot it does not own
	node->heap_index = (node->heap_index + 1) % intact.node_nb;
	assert(lrutci_validate(&c) == false);
	*node = saved;

	// a head that claims a predecessor
	node->lru_prev = node;
	assert(lrutci_validate(&c) == false);
	*node = saved;

	// a back link that does not match its predecessor
	middle->lru_prev = middle;
	assert(lrutci_validate(&c) == false);
	*middle = middle_saved;

	// a list shorter than node_nb
	c.LRU->lru_prev->lru_next = NULL;
	assert(lrutci_validate(&c) == false);
	c.LRU->lru_prev->lru_next = c.LRU;

	// LRU that is not the list tail
	c.LRU = middle;
	assert(lrutci_validate(&c) == false);
	c = intact;

	// a recency list cycle: the bounded walk must not hang
	node->lru_prev  = c.LRU;
	c.LRU->lru_next = node;
	assert(lrutci_validate(&c) == false);
	*node           = saved;
	c.LRU->lru_next = NULL;
	assert(lrutci_validate(&c) == true);

	// a bucket cycle
	h          = lrutci_hash_djb2(node->kv.key, node->kv.k_len) % c.buckets_nb;
	saved_head = c.buckets[h];
	saved_next = node->hash_next;
	node->hash_next = node;
	assert(c.buckets[h] == node);
	assert(lrutci_validate(&c) == false);
	node->hash_next = saved_next;
	c.buckets[h]    = saved_head;

	// a bucket node the heap does not track, same count
	c.buckets[h] = &ghost;
	assert(lrutci_validate(&c) == false);
	c.buckets[h] = saved_head;

	// fewer nodes in the buckets than node_nb
	c.buckets[h] = NULL;
	assert(lrutci_validate(&c) == false);
	c.buckets[h] = saved_head;
	assert(lrutci_validate(&c) == true);

	// reserved nodes are not heap tracked
	lrutc_remove(&c, "a", 1);
	assert(lrutci_validate(&c) == true);
	assert(c.reserved_nodes != NULL);
	assert(c.reserved_nodes->heap_index == C_MINI__LRUTC_NO_HEAP_SLOT);
	c.reserved_nodes->heap_index = 0;
	assert(lrutci_validate(&c) == false);
	c.reserved_nodes->heap_index = C_MINI__LRUTC_NO_HEAP_SLOT;
	assert(lrutci_validate(&c) == true);

	assert(lrutci_validate(NULL) == false);
	lrutc_destroy(&c);
	return;
}
/**
 * \brief unlinking a node the heap does not track must not evict an unrelated
 * entry from the expiry order
**/
NO_COV void test_cm_lrutci_unlink_untracked(void) {
	t_cm_lrutc          c;
	t_cm_lrutc_keynode *popped;

	lrutc_init(&c, 2, NULL);
	assert(lrutc_put(&c, "a", 1, "A") == true);
	assert(lrutc_put(&c, "b", 1, "B") == true);
	popped = lrutci_hp_pop(&c); // detach the expiry root only
	assert(popped != NULL);
	assert(popped->heap_index == C_MINI__LRUTC_NO_HEAP_SLOT);
	assert(c.node_nb == 1);
	lrutci_unlink_node(&c, popped); // must skip the heap, not pop slot 0
	assert(c.node_nb == 1);
	assert(lrutci_rsrvd_push(&c, popped) == true); // hand the detached node back
	assert(lrutc_has(&c, "a", 1) + lrutc_has(&c, "b", 1) == 1);
	assert(lrutci_validate(&c) == true);
	lrutc_destroy(&c);

	// same, with an in-range index that another node owns
	lrutc_init(&c, 2, NULL);
	assert(lrutc_put(&c, "a", 1, "A") == true);
	assert(lrutc_put(&c, "b", 1, "B") == true);
	popped = lrutci_hp_pop(&c);
	assert(popped != NULL);
	popped->heap_index = 0; // valid range, minHeap[0] is the other node
	lrutci_unlink_node(&c, popped);
	assert(c.node_nb == 1);
	assert(lrutci_rsrvd_push(&c, popped) == true); // hand the detached node back
	assert(lrutci_validate(&c) == true);
	lrutc_destroy(&c);
	return;
}
