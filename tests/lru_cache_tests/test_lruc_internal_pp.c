#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lru_cache_tests.h"

void test_lruci_hsh_pop(void) {
	t_lruc          c;
	const char     *key1 = "hsh_pop key";
	const char     *key2 = "different key";
	t_lruc_keynode *kvn_ptr;
	t_lruc_keynode  kvn_bad = (t_lruc_keynode){
	     .hash_next  = NULL,
	     .lru_next   = NULL,
	     .heap_index = 0,
	     .lru_prev   = NULL,
	     .time       = lruci_timenow(),
	     .kv =
            (t_keyvalue){
	                     .k_len = strlen(key2),
	                     .key   = key2,
	                     .value = (void *)0xDEADCAFE,
	                     },
    };
	size_t hash_kvn_bad;
	size_t hash_MRU;
	size_t invalid_MRU_hash;

	lruc_init(&c, 3, NULL);
	lruc_put(&c, key1, strlen(key1), "value hsh_pop");
	assert(c.MRU != NULL), assert(c.MRU->kv.key == key1);
	hash_kvn_bad     = lruci_hash_djb2(kvn_bad.kv.key, kvn_bad.kv.k_len);
	hash_MRU         = lruci_hash_djb2(c.MRU->kv.key, c.MRU->kv.k_len);
	invalid_MRU_hash = hash_MRU + 1;
	assert(c.buckets[invalid_MRU_hash % c.buckets_nb] == NULL);

	kvn_ptr = c.MRU;
	assert(lruci_hsh_pop(&c, kvn_ptr, hash_MRU) == kvn_ptr);
	kvn_ptr->hash_next                 = c.buckets[hash_MRU % c.buckets_nb];
	c.buckets[hash_MRU % c.buckets_nb] = kvn_ptr;

	kvn_ptr = c.MRU;
	assert(lruci_hsh_pop(&c, kvn_ptr, invalid_MRU_hash) == NULL);

	kvn_ptr = &kvn_bad;
	assert(lruci_hsh_pop(&c, kvn_ptr, hash_kvn_bad) == NULL);

	free(c.MRU);
	free(c.buckets);
}
