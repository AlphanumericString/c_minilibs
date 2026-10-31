#include <assert.h>
#include <string.h>

#include "lru_cache_tests.h"

NO_COV void test_lruci_keyinhash(void) {
	t_lruc      c;
	size_t      returnd_hash, actual_hash;
	const char *key1 = "big key";
	const char *key2 = "small key";
	const char *key3 = "skull key";
	const char *key4 = "void key";

	const char *val1 = "big door";
	const char *val2 = "small door";
	const char *val3 = "skull door";

	lruc_init(&c, 3, NULL);
	lruc_put(&c, key1, strlen(key1), val1);
	lruc_put(&c, key2, strlen(key2), val2);
	lruc_put(&c, key3, strlen(key3), val3);
	assert(lruci_keyinhash(&c, key1, strlen(key1), NULL) != NULL);
	assert(lruci_keyinhash(&c, key1, strlen(key1), &returnd_hash) != NULL);
	actual_hash = lruci_hash_djb2(key1, strlen(key1));
	assert(returnd_hash == actual_hash % c.buckets_nb);
	assert(lruci_keyinhash(&c, key2, strlen(key2), &returnd_hash) != NULL);
	actual_hash = lruci_hash_djb2(key2, strlen(key2));
	assert(returnd_hash == actual_hash % c.buckets_nb);
	assert(lruci_keyinhash(&c, key3, strlen(key3), &returnd_hash) != NULL);
	actual_hash = lruci_hash_djb2(key3, strlen(key3));
	assert(returnd_hash == actual_hash % c.buckets_nb);
	assert(lruci_keyinhash(&c, key4, strlen(key4), &returnd_hash) == NULL);
	actual_hash = lruci_hash_djb2(key4, strlen(key4));
	assert(returnd_hash == actual_hash % c.buckets_nb);
	lruc_destroy(&c);
}
NO_COV void test_lruci_keyinlist(void) {
	t_lruc      c;
	const char *key1 = "big key";
	const char *val1 = "big door";
	const char *key2 = "small key";
	const char *val2 = "small door";
	const char *key3 = "skull key";
	const char *val3 = "skull door";
	const char *key4 = "void key";

	lruc_init(&c, 3, NULL);
	lruc_put(&c, key1, strlen(key1), val1);
	lruc_put(&c, key2, strlen(key2), val2);
	lruc_put(&c, key3, strlen(key3), val3);
	assert(lruci_keyinlist(&c, key1, strlen(key1)) != NULL);
	assert(lruci_keyinlist(&c, key2, strlen(key2)) != NULL);
	assert(lruci_keyinlist(&c, key3, strlen(key3)) != NULL);
	assert(lruci_keyinlist(&c, key4, strlen(key4)) == NULL);
	lruc_destroy(&c);
}
NO_COV void test_lruci_node_eq(void) {
	const char    *key1 = "aaaaAAAAAAAAAAAAAAAAa";
	const char    *key2 = "bbbbBBBBBBBBBBBBBbbbb";
	t_lruc_keynode kv1, kv2;

	assert(lruci_node_eq(&kv1, &kv1) == true);
	assert(lruci_node_eq(&kv2, &kv2) == true);
	kv1.kv.key   = key1;
	kv1.kv.k_len = 21;
	kv2.kv       = kv1.kv;
	assert(lruci_node_eq(&kv1, &kv2) == true);
	kv1.kv.key = key2;
	assert(lruci_node_eq(&kv1, &kv2) == false);
}
