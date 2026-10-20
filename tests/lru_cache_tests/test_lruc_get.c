#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache_tests.h"

NO_COV void test_lruc_get_lru(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_lruc      c, intact_copy;

	v1 = strdup("toto_1");
	v2 = strdup("toto_2");
	v3 = strdup("toto_3");
	v4 = strdup("toto_4");
	lruc_init(&c, 2, free);
	lruc_setttl(&c, 300);
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
NO_COV void test_lruc_get(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_lruc      c, intact_copy;

	v1 = strdup("toto_1");
	v2 = strdup("toto_2");
	v3 = strdup("toto_3");
	v4 = strdup("toto_4");
	lruc_init(&c, 2, free);
	lruc_setttl(&c, 300);
	lruc_put(&c, k1, strlen(k1), v1);
	lruc_put(&c, k2, strlen(k2), v2);
	assert(c.LRU != NULL);
	assert(c.MRU != NULL);
	assert(c.minHeap[0] != NULL);
	assert(c.LRU->kv.key == k1);
	assert(c.MRU->kv.key == k2);
	assert(lruc_get(&c, k1, strlen(k1)) == v1);
	assert(lruc_get(&c, k2, strlen(k2)) == v2);
	usleep(400 * 1000); // 1ms = 1k us
	assert(lruc_get(&c, k1, strlen(k1)) == NULL);
	assert(lruc_get(&c, k2, strlen(k2)) == NULL);
	lruc_put(&c, k3, strlen(k3), v3);
	lruc_put(&c, k4, strlen(k4), v4);
	assert(lruc_get(&c, k1, strlen(k1)) == NULL); // were replaced
	assert(lruc_get(&c, k2, strlen(k2)) == NULL);
	assert(lruc_get(&c, k3, strlen(k3)) == v3); // works
	assert(lruc_get(&c, k4, strlen(k4)) == v4);
	intact_copy = c;
	assert(lruc_get(NULL, k4, strlen(k4)) == NULL);
	c.MRU = NULL;
	assert(lruc_get(&c, k4, strlen(k4)) == NULL);
	c         = intact_copy;
	c.node_nb = 0;
	assert(lruc_get(&c, k4, strlen(k4)) == NULL);
	c         = intact_copy;
	c.buckets = NULL;
	assert(lruc_get(&c, k4, strlen(k4)) == NULL);
	c = intact_copy;
	assert(lruc_get(&c, k4, strlen(k4)) == v4);

	lruc_destroy(&c);
	return;
}
