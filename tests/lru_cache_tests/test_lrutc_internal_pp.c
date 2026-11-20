#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lrutc_tests.h"

NO_COV void test_cm_lrutci_rsvrd_push(void) {
	t_cm_lrutc         c  = (t_cm_lrutc){0};
	t_cm_lrutc_keynode kv = (t_cm_lrutc_keynode){
	    .hash_next    = NULL,
	    .lru_next     = NULL,
	    .heap_index   = 0,
	    .lru_prev     = NULL,
	    .time_emitted = lrutci_timenow(),
	    .kv           = (t_cm_keyvalue){0},
	};

	assert(c.reserved_nodes == NULL);
	assert(lrutci_rsrvd_push(&c, &kv));
	assert(c.reserved_nodes == &kv);
}
NO_COV void test_cm_lrutci_rsvrd_pop(void) {
	t_cm_lrutc         c  = (t_cm_lrutc){0};
	t_cm_lrutc_keynode kv = (t_cm_lrutc_keynode){
	    .hash_next    = NULL,
	    .lru_next     = NULL,
	    .heap_index   = 0,
	    .lru_prev     = NULL,
	    .time_emitted = lrutci_timenow(),
	    .kv           = (t_cm_keyvalue){0},
	};

	c.reserved_nodes = &kv;
	assert(lrutci_rsrvd_pop(&c) == &kv);
	assert(c.reserved_nodes == NULL);
	assert(lrutci_rsrvd_pop(&c) == NULL);
}

NO_COV void test_cm_lrutci_hsh_pop(void) {
	t_cm_lrutc          c;
	const char         *key1 = "hsh_pop key";
	const char         *key2 = "different key";
	t_cm_lrutc_keynode *kvn_ptr;
	t_cm_lrutc_keynode  kvn_bad = (t_cm_lrutc_keynode){
	     .hash_next    = NULL,
	     .lru_next     = NULL,
	     .heap_index   = 0,
	     .lru_prev     = NULL,
	     .time_emitted = lrutci_timenow(),
	     .kv =
            (t_cm_keyvalue){
	                        .k_len = strlen(key2),
	                        .key   = key2,
	                        .value = (void *)0xDEADCAFE,
	                        },
    };
	size_t hash_kvn_bad;
	size_t hash_MRU;
	size_t invalid_MRU_hash;

	lrutc_init(&c, 3, NULL);
	lrutc_put(&c, key1, strlen(key1), "value hsh_pop");
	assert(c.MRU != NULL), assert(c.MRU->kv.key == key1);
	hash_kvn_bad     = lrutci_hash_djb2(kvn_bad.kv.key, kvn_bad.kv.k_len);
	hash_MRU         = lrutci_hash_djb2(c.MRU->kv.key, c.MRU->kv.k_len);
	invalid_MRU_hash = hash_MRU + 1;
	assert(c.buckets[invalid_MRU_hash % c.buckets_nb] == NULL);

	kvn_ptr = c.MRU;
	assert(lrutci_hsh_pop(&c, kvn_ptr, hash_MRU) == kvn_ptr);
	kvn_ptr->hash_next                 = c.buckets[hash_MRU % c.buckets_nb];
	c.buckets[hash_MRU % c.buckets_nb] = kvn_ptr;

	kvn_ptr = c.MRU;
	assert(lrutci_hsh_pop(&c, kvn_ptr, invalid_MRU_hash) == NULL);

	kvn_ptr = &kvn_bad;
	assert(lrutci_hsh_pop(&c, kvn_ptr, hash_kvn_bad) == NULL);

	free(c.MRU);
	free(c.buckets);
}
