#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache_tests.h"

static int gen_calls = 0;

static void *gen_count(void *arg) {
	gen_calls++;
	return (arg);
}
static void *gen_null(void *arg) {
	(void)arg;
	return (NULL);
}

NO_COV void test_cm_lrutc_get_lru(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_cm_lrutc   c, intact_copy;

	v1 = strdup("toto_1");
	v2 = strdup("toto_2");
	v3 = strdup("toto_3");
	v4 = strdup("toto_4");
	lrutc_init(&c, 2, free);
	lrutc_setttl(&c, LRUC_MS_TO_NS(300));
	lrutc_put(&c, k1, strlen(k1), v1);
	lrutc_put(&c, k2, strlen(k2), v2);
	assert(c.LRU != NULL);
	assert(c.MRU != NULL);
	assert(c.minHeap[0] != NULL);
	assert(c.MRU != NULL && c.MRU->kv.key == k2);
	assert(c.LRU != NULL && c.LRU->kv.key == k1);
	assert(c.LRU->lru_prev == c.MRU && c.MRU->lru_next == c.LRU);
	assert(lrutc_get_lru(&c, k1, strlen(k1)) == v1);
	assert(c.MRU != NULL && c.MRU->kv.key == k1);
	assert(c.LRU != NULL && c.LRU->kv.key == k2);
	assert(c.LRU->lru_prev == c.MRU && c.MRU->lru_next == c.LRU);
	assert(lrutc_get_lru(&c, k2, strlen(k2)) == v2);
	assert(c.MRU != NULL && c.MRU->kv.key == k2);
	assert(c.LRU != NULL && c.LRU->kv.key == k1);
	assert(c.LRU->lru_prev == c.MRU && c.MRU->lru_next == c.LRU);
	lrutc_put(&c, k3, strlen(k3), v3);
	assert(c.MRU != NULL && c.MRU->kv.key == k3);
	assert(c.LRU != NULL && c.LRU->kv.key == k2);
	assert(c.LRU->lru_prev == c.MRU && c.MRU->lru_next == c.LRU);
	lrutc_put(&c, k4, strlen(k4), v4); // v2 + v1 should not leak
	assert(c.MRU != NULL && c.MRU->kv.key == k4);
	assert(c.LRU != NULL && c.LRU->kv.key == k3);
	assert(c.LRU->lru_prev == c.MRU && c.MRU->lru_next == c.LRU);
	assert(lrutc_get_lru(&c, k3, strlen(k3)) == v3);
	assert(c.MRU != NULL && c.MRU->kv.key == k3);
	assert(c.LRU != NULL && c.LRU->kv.key == k4);
	assert(c.LRU->lru_prev == c.MRU && c.MRU->lru_next == c.LRU);
	assert(lrutc_get_lru(&c, k4, strlen(k4)) == v4);
	lrutc_test_sleep_ms(400);
	assert(lrutc_get_lru(&c, k3, strlen(k3)) == v3);
	assert(lrutc_get_lru(&c, k4, strlen(k4)) == v4);
	assert(lrutc_get_lru(&c, "unknown", 7) == NULL); // present cache, absent key
	intact_copy = c;

	c.MRU = NULL;
	assert(lrutc_get_lru(&c, k3, strlen(k3)) == NULL);
	c = intact_copy;

	c.node_nb = 0;
	assert(lrutc_get_lru(&c, k3, strlen(k3)) == NULL);
	c = intact_copy;

	assert(lrutc_get_lru(NULL, k3, strlen(k3)) == NULL);

	lrutc_destroy(&c);
	return;
}
NO_COV void test_cm_lrutc_try_get(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_cm_lrutc   c, intact_copy;

	v1 = strdup("toto_1");
	v2 = strdup("toto_2");
	v3 = strdup("toto_3");
	v4 = strdup("toto_4");
	lrutc_init(&c, 2, free);
	lrutc_setttl(&c, LRUC_MS_TO_NS(300));
	lrutc_put(&c, k1, strlen(k1), v1);
	lrutc_put(&c, k2, strlen(k2), v2);
	assert(c.LRU != NULL);
	assert(c.MRU != NULL);
	assert(c.minHeap[0] != NULL);
	assert(c.LRU->kv.key == k1);
	assert(c.MRU->kv.key == k2);
	assert(lrutc_try_get(&c, k1, strlen(k1)) == v1);
	assert(lrutc_try_get(&c, k2, strlen(k2)) == v2);
	lrutc_test_sleep_ms(400);
	assert(lrutc_try_get(&c, k1, strlen(k1)) == NULL);
	assert(lrutc_try_get(&c, k2, strlen(k2)) == NULL);
	lrutc_put(&c, k3, strlen(k3), v3);
	lrutc_put(&c, k4, strlen(k4), v4);
	assert(lrutc_try_get(&c, k1, strlen(k1)) == NULL); // were replaced
	assert(lrutc_try_get(&c, k2, strlen(k2)) == NULL);
	assert(lrutc_try_get(&c, k3, strlen(k3)) == v3); // works
	assert(lrutc_try_get(&c, k4, strlen(k4)) == v4);
	intact_copy = c;
	assert(lrutc_try_get(NULL, k4, strlen(k4)) == NULL);
	c.MRU = NULL;
	assert(lrutc_try_get(&c, k4, strlen(k4)) == NULL);
	c         = intact_copy;
	c.node_nb = 0;
	assert(lrutc_try_get(&c, k4, strlen(k4)) == NULL);
	c         = intact_copy;
	c.buckets = NULL;
	assert(lrutc_try_get(&c, k4, strlen(k4)) == NULL);
	c = intact_copy;
	assert(lrutc_try_get(&c, k4, strlen(k4)) == v4);

	clock_set_call_limit(0);
	assert(lrutc_try_get(&c, k4, strlen(k4)) == NULL);
	clock_set_call_limit(CLOCK_DFT_LIMIT);

	lrutc_destroy(&c);
	return;
}
NO_COV void test_cm_lrutc_get(void) {
	const char *k1 = "abc", *k2 = "def";
	void       *v;
	t_cm_lrutc   c;

	gen_calls = 0;
	lrutc_init(&c, 2, NULL);

	// miss -> computes and stores
	v = lrutc_get(&c, k1, strlen(k1), gen_count, (void *)k1);
	assert(v == (void *)k1);
	assert(gen_calls == 1);
	assert(lrutc_has(&c, k1, strlen(k1)) == true);
	assert(lrutc_size(&c) == 1);

	// hit -> returns the stored value without computing
	v = lrutc_get(&c, k1, strlen(k1), gen_count, (void *)k1);
	assert(v == (void *)k1);
	assert(gen_calls == 1);

	// miss + no generator
	assert(lrutc_get(&c, k2, strlen(k2), NULL, NULL) == NULL);
	assert(lrutc_has(&c, k2, strlen(k2)) == false);

	// miss + generator returning NULL
	assert(lrutc_get(&c, k2, strlen(k2), gen_null, (void *)k2) == NULL);
	assert(lrutc_has(&c, k2, strlen(k2)) == false);

	// errors
	assert(lrutc_get(NULL, k1, strlen(k1), gen_count, (void *)k1) == NULL);

	// compute succeeds but storing fails (clock error on put)
	lrutc_setttl(&c, LRUC_MS_TO_NS(100));
	clock_set_call_limit(0);
	assert(lrutc_get(&c, k2, strlen(k2), gen_count, (void *)k2) == NULL);
	clock_set_call_limit(CLOCK_DFT_LIMIT);
	lrutc_setttl(&c, C_MINI__LRUTC_TTL_DFLT);

	lrutc_destroy(&c);
	return;
}
NO_COV void test_cm_lrutc_getttl(void) {
	const char *k1 = "abc", *k2 = "def";
	void       *v;
	t_cm_lrutc   c;

	gen_calls = 0;
	lrutc_init(&c, 4, NULL);

	// ttl 0 delegates to lrutc_get (miss -> computes)
	v = lrutc_getttl(&c, k1, strlen(k1), gen_count, (void *)k1, 0);
	assert(v == (void *)k1);
	assert(gen_calls == 1);
	assert(lrutc_has(&c, k1, strlen(k1)) == true);

	// present but no ttl -> recompute and apply the ttl
	v = lrutc_getttl(&c, k1, strlen(k1), gen_count, (void *)k1,
	                LRUC_MS_TO_NS(200));
	assert(v == (void *)k1);
	assert(gen_calls == 2);

	// present, has ttl, not expired -> hit, no compute
	v = lrutc_getttl(&c, k1, strlen(k1), gen_count, (void *)k1,
	                LRUC_MS_TO_NS(200));
	assert(v == (void *)k1);
	assert(gen_calls == 2);
	assert(lrutc_isexpired(&c, k1, strlen(k1)) == false);

	// expired -> recompute
	lrutc_test_sleep_ms(250);
	v = lrutc_getttl(&c, k1, strlen(k1), gen_count, (void *)k1,
	                LRUC_MS_TO_NS(200));
	assert(v == (void *)k1);
	assert(gen_calls == 3);
	assert(lrutc_isexpired(&c, k1, strlen(k1)) == false);

	// miss + no generator
	assert(lrutc_getttl(&c, k2, strlen(k2), NULL, NULL, LRUC_MS_TO_NS(200)) ==
	       NULL);
	assert(lrutc_has(&c, k2, strlen(k2)) == false);

	// miss + generator returning NULL
	assert(lrutc_getttl(&c, k2, strlen(k2), gen_null, (void *)k2,
	                   LRUC_MS_TO_NS(200)) == NULL);
	assert(lrutc_has(&c, k2, strlen(k2)) == false);

	// errors
	assert(lrutc_getttl(NULL, k1, strlen(k1), gen_count, (void *)k1,
	                   LRUC_MS_TO_NS(200)) == NULL);

	// clock failure while checking a present, ttl'd entry
	clock_set_call_limit(0);
	assert(lrutc_getttl(&c, k1, strlen(k1), gen_count, (void *)k1,
	                   LRUC_MS_TO_NS(200)) == NULL);
	clock_set_call_limit(CLOCK_DFT_LIMIT);

	// compute succeeds but storing fails (clock error on put)
	lrutc_clear(&c);
	clock_set_call_limit(0);
	assert(lrutc_getttl(&c, k2, strlen(k2), gen_count, (void *)k2,
	                   LRUC_MS_TO_NS(200)) == NULL);
	clock_set_call_limit(CLOCK_DFT_LIMIT);

	lrutc_destroy(&c);
	return;
}
