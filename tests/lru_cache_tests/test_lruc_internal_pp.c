#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lru_cache_tests.h"

// TODO: tests for :lruci_hsh_pop, lruci_hsh_push
void test_lruci_hsh_pop(void) {
	t_lruc          c;
	const char     *key1 = "hsh_pop key";
	t_lruc_keynode *kvn_ok;

	lruc_init(&c, 3, NULL);
	lruc_put(&c, key1, strlen(key1), "value hsh_pop");
	assert(c.MRU != NULL), assert(c.MRU->kv.key == key1);
	kvn_ok = c.MRU;
	assert(lruci_hsh_pop(&c, kvn_ok,
	                     lruci_hash_djb2(kvn_ok->kv.key, kvn_ok->kv.k_len)) ==
	       kvn_ok);
	// kvn bad
	// kvn bad hash

	free(kvn_ok);
	free(c.buckets);
}
void test_lruci_hsh_push(void) {
	t_lruc c;

	lruc_init(&c, 3, NULL);
	lruc_destroy(&c);
}
