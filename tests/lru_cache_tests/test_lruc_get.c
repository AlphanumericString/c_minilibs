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

NO_COV void test_cm_lruc_get_lru(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_cm_lruc   c, intact_copy;

	v1 = strdup("toto_1");
	v2 = strdup("toto_2");
	v3 = strdup("toto_3");
	v4 = strdup("toto_4");
	lruc_init(&c, 2, free);
	lruc_setttl(&c, LRUC_MS_TO_NS(300));
	lruc_put(&c, k1, strlen(k1), v1);
	lruc_put(&c, k2, strlen(k2), v2);
	assert(c.LRU != NULL);
	assert(c.MRU != NULL);
	assert(c.minHeap[0] != NULL);
	assert(c.MRU != NULL && c.MRU->kv.key == k2);
	assert(c.LRU != NULL && c.LRU->kv.key == k1);
	assert(c.LRU->lru_prev == c.MRU && c.MRU->lru_next == c.LRU);
	assert(lruc_get_lru(&c, k1, strlen(k1)) == v1);
	assert(c.MRU != NULL && c.MRU->kv.key == k1);
	assert(c.LRU != NULL && c.LRU->kv.key == k2);
	assert(c.LRU->lru_prev == c.MRU && c.MRU->lru_next == c.LRU);
	assert(lruc_get_lru(&c, k2, strlen(k2)) == v2);
	assert(c.MRU != NULL && c.MRU->kv.key == k2);
	assert(c.LRU != NULL && c.LRU->kv.key == k1);
	assert(c.LRU->lru_prev == c.MRU && c.MRU->lru_next == c.LRU);
	lruc_put(&c, k3, strlen(k3), v3);
	assert(c.MRU != NULL && c.MRU->kv.key == k3);
	assert(c.LRU != NULL && c.LRU->kv.key == k2);
	assert(c.LRU->lru_prev == c.MRU && c.MRU->lru_next == c.LRU);
	lruc_put(&c, k4, strlen(k4), v4); // v2 + v1 should not leak
	assert(c.MRU != NULL && c.MRU->kv.key == k4);
	assert(c.LRU != NULL && c.LRU->kv.key == k3);
	assert(c.LRU->lru_prev == c.MRU && c.MRU->lru_next == c.LRU);
	assert(lruc_get_lru(&c, k3, strlen(k3)) == v3);
	assert(c.MRU != NULL && c.MRU->kv.key == k3);
	assert(c.LRU != NULL && c.LRU->kv.key == k4);
	assert(c.LRU->lru_prev == c.MRU && c.MRU->lru_next == c.LRU);
	assert(lruc_get_lru(&c, k4, strlen(k4)) == v4);
	usleep(400 * 1000); // 1ms = 1k us
	assert(lruc_get_lru(&c, k3, strlen(k3)) == v3);
	assert(lruc_get_lru(&c, k4, strlen(k4)) == v4);
	intact_copy = c;

	c.MRU = NULL;
	assert(lruc_get_lru(&c, k3, strlen(k3)) == NULL);
	c = intact_copy;

	c.node_nb = 0;
	assert(lruc_get_lru(&c, k3, strlen(k3)) == NULL);
	c = intact_copy;

	assert(lruc_get_lru(NULL, k3, strlen(k3)) == NULL);

	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_try_get(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_cm_lruc   c, intact_copy;

	v1 = strdup("toto_1");
	v2 = strdup("toto_2");
	v3 = strdup("toto_3");
	v4 = strdup("toto_4");
	lruc_init(&c, 2, free);
	lruc_setttl(&c, LRUC_MS_TO_NS(300));
	lruc_put(&c, k1, strlen(k1), v1);
	lruc_put(&c, k2, strlen(k2), v2);
	assert(c.LRU != NULL);
	assert(c.MRU != NULL);
	assert(c.minHeap[0] != NULL);
	assert(c.LRU->kv.key == k1);
	assert(c.MRU->kv.key == k2);
	assert(lruc_try_get(&c, k1, strlen(k1)) == v1);
	assert(lruc_try_get(&c, k2, strlen(k2)) == v2);
	usleep(400 * 1000); // 1ms = 1k us
	assert(lruc_try_get(&c, k1, strlen(k1)) == NULL);
	assert(lruc_try_get(&c, k2, strlen(k2)) == NULL);
	lruc_put(&c, k3, strlen(k3), v3);
	lruc_put(&c, k4, strlen(k4), v4);
	assert(lruc_try_get(&c, k1, strlen(k1)) == NULL); // were replaced
	assert(lruc_try_get(&c, k2, strlen(k2)) == NULL);
	assert(lruc_try_get(&c, k3, strlen(k3)) == v3); // works
	assert(lruc_try_get(&c, k4, strlen(k4)) == v4);
	intact_copy = c;
	assert(lruc_try_get(NULL, k4, strlen(k4)) == NULL);
	c.MRU = NULL;
	assert(lruc_try_get(&c, k4, strlen(k4)) == NULL);
	c         = intact_copy;
	c.node_nb = 0;
	assert(lruc_try_get(&c, k4, strlen(k4)) == NULL);
	c         = intact_copy;
	c.buckets = NULL;
	assert(lruc_try_get(&c, k4, strlen(k4)) == NULL);
	c = intact_copy;
	assert(lruc_try_get(&c, k4, strlen(k4)) == v4);

	clock_set_call_limit(0);
	assert(lruc_try_get(&c, k4, strlen(k4)) == NULL);
	clock_set_call_limit(CLOCK_DFT_LIMIT);

	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_get(void) {
	const char *k1 = "abc", *k2 = "def";
	void       *v;
	t_cm_lruc   c;

	gen_calls = 0;
	lruc_init(&c, 2, NULL);

	// miss -> computes and stores
	v = lruc_get(&c, k1, strlen(k1), gen_count, (void *)k1);
	assert(v == (void *)k1);
	assert(gen_calls == 1);
	assert(lruc_has(&c, k1, strlen(k1)) == true);
	assert(lruc_size(&c) == 1);

	// hit -> returns the stored value without computing
	v = lruc_get(&c, k1, strlen(k1), gen_count, (void *)k1);
	assert(v == (void *)k1);
	assert(gen_calls == 1);

	// miss + no generator
	assert(lruc_get(&c, k2, strlen(k2), NULL, NULL) == NULL);
	assert(lruc_has(&c, k2, strlen(k2)) == false);

	// miss + generator returning NULL
	assert(lruc_get(&c, k2, strlen(k2), gen_null, (void *)k2) == NULL);
	assert(lruc_has(&c, k2, strlen(k2)) == false);

	// errors
	assert(lruc_get(NULL, k1, strlen(k1), gen_count, (void *)k1) == NULL);

	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_getttl(void) {
	const char *k1 = "abc", *k2 = "def";
	void       *v;
	t_cm_lruc   c;

	gen_calls = 0;
	lruc_init(&c, 4, NULL);

	// ttl 0 delegates to lruc_get (miss -> computes)
	v = lruc_getttl(&c, k1, strlen(k1), gen_count, (void *)k1, 0);
	assert(v == (void *)k1);
	assert(gen_calls == 1);
	assert(lruc_has(&c, k1, strlen(k1)) == true);

	// present but no ttl -> recompute and apply the ttl
	v = lruc_getttl(&c, k1, strlen(k1), gen_count, (void *)k1,
	                LRUC_MS_TO_NS(200));
	assert(v == (void *)k1);
	assert(gen_calls == 2);

	// present, has ttl, not expired -> hit, no compute
	v = lruc_getttl(&c, k1, strlen(k1), gen_count, (void *)k1,
	                LRUC_MS_TO_NS(200));
	assert(v == (void *)k1);
	assert(gen_calls == 2);
	assert(lruc_isexpired(&c, k1, strlen(k1)) == false);

	// expired -> recompute
	usleep(250 * 1000);
	v = lruc_getttl(&c, k1, strlen(k1), gen_count, (void *)k1,
	                LRUC_MS_TO_NS(200));
	assert(v == (void *)k1);
	assert(gen_calls == 3);
	assert(lruc_isexpired(&c, k1, strlen(k1)) == false);

	// miss + no generator
	assert(lruc_getttl(&c, k2, strlen(k2), NULL, NULL, LRUC_MS_TO_NS(200)) ==
	       NULL);
	assert(lruc_has(&c, k2, strlen(k2)) == false);

	// miss + generator returning NULL
	assert(lruc_getttl(&c, k2, strlen(k2), gen_null, (void *)k2,
	                   LRUC_MS_TO_NS(200)) == NULL);
	assert(lruc_has(&c, k2, strlen(k2)) == false);

	// errors
	assert(lruc_getttl(NULL, k1, strlen(k1), gen_count, (void *)k1,
	                   LRUC_MS_TO_NS(200)) == NULL);

	lruc_destroy(&c);
	return;
}
