#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lruc_tests.h"

static int gen_calls = 0;

static void *gen_count(void *arg) {
	gen_calls++;
	return (arg);
}
static void *gen_null(void *arg) {
	(void)arg;
	return (NULL);
}

NO_COV void test_cm_lruc_try_get(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi";
	void       *v1 = "1", *v2 = "2";
	t_cm_lruc   c, intact_copy;

	lruc_init(&c, 2, NULL);
	lruc_put(&c, k1, strlen(k1), v1);
	lruc_put(&c, k2, strlen(k2), v2);
	assert(lruc_try_get(&c, k1, strlen(k1)) == v1);
	// a hit promotes the entry to most recently used
	assert(c.MRU->kv.key == k1 && c.LRU->kv.key == k2);
	assert(lruc_hits(&c) == 1);

	// a miss is counted, nothing is stored
	assert(lruc_try_get(&c, k3, strlen(k3)) == NULL);
	assert(lruc_misses(&c) == 1);
	assert(lruc_has(&c, k3, strlen(k3)) == false);

	intact_copy = c;
	c.MRU = NULL;
	assert(lruc_try_get(&c, k1, strlen(k1)) == NULL);
	c         = intact_copy;
	c.node_nb = 0;
	assert(lruc_try_get(&c, k1, strlen(k1)) == NULL);
	c         = intact_copy;
	c.buckets = NULL;
	assert(lruc_try_get(&c, k1, strlen(k1)) == NULL);
	c = intact_copy;
	assert(lruc_try_get(NULL, k1, strlen(k1)) == NULL);

	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_get(void) {
	const char *k1 = "abc", *k2 = "def";
	t_cm_lruc   c;

	gen_calls = 0;
	lruc_init(&c, 2, NULL);

	// miss -> computes and stores
	assert(lruc_get(&c, k1, strlen(k1), gen_count, (void *)k1) == (void *)k1);
	assert(gen_calls == 1);
	assert(lruc_has(&c, k1, strlen(k1)) == true);
	assert(lruc_size(&c) == 1);

	// hit -> returns the stored value without computing
	assert(lruc_get(&c, k1, strlen(k1), gen_count, (void *)k1) == (void *)k1);
	assert(gen_calls == 1);

	// miss + no generator
	assert(lruc_get(&c, k2, strlen(k2), NULL, NULL) == NULL);
	assert(lruc_has(&c, k2, strlen(k2)) == false);

	// miss + generator returning NULL
	assert(lruc_get(&c, k2, strlen(k2), gen_null, (void *)k2) == NULL);
	assert(lruc_has(&c, k2, strlen(k2)) == false);

	// compute succeeds but the cache is unusable
	void *buckets = c.buckets;
	c.buckets     = NULL;
	assert(lruc_get(&c, k2, strlen(k2), gen_count, (void *)k2) == NULL);
	c.buckets = buckets;
	lruc_destroy(&c);

	// compute succeeds but storing fails
	lruc_init(&c, 1, NULL);
	alloc_set_call_limit(0);
	assert(lruc_get(&c, k2, strlen(k2), gen_count, (void *)k2) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	assert(lruc_get(NULL, k1, strlen(k1), gen_count, (void *)k1) == NULL);
	lruc_destroy(&c);
	return;
}
