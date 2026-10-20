#include <assert.h>
#include <string.h>

#include "lru_cache_tests.h"

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
