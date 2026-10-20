#include <assert.h>
#include <string.h>

#include "lru_cache_tests.h"

void blank(void *t) {
	(void)t;
}

NO_COV void test_lruc_clear(void) {
	t_lruc c, intact_copy;
	char   empty_buff[sizeof(t_lruc_keynode *) * 32] = {0};

	{ // lru full
		lruc_init(&c, 2, blank);
		lruc_put(&c, test_lruc_clear, sizeof(test_lruc_clear), "his address");
		lruc_get(&c, test_lruc_clear, sizeof(test_lruc_clear));
		lruc_clear(&c);
		assert(c.LRU == NULL);
		assert(c.MRU == NULL);
		assert(c.node_nb == 0);
		assert(c.buckets_nb == 2);
		assert(c.dtor == blank);
		assert(!memcmp(c.buckets, empty_buff, sizeof(t_lruc_keynode *) * 2));
		lruc_destroy(&c);
	}
	{ // lru with large holes
		lruc_init(&c, 32, NULL);
		lruc_put(&c, test_lruc_clear, sizeof(test_lruc_clear), "his address");
		lruc_clear(&c);
		assert(c.LRU == NULL);
		assert(c.MRU == NULL);
		assert(c.node_nb == 0);
		assert(c.buckets_nb == 32);
		assert(!memcmp(c.buckets, empty_buff, sizeof(t_lruc_keynode *) * (32)));
		lruc_destroy(&c);
	}
	{ // errors
		lruc_init(&c, 32, NULL);
		lruc_put(&c, test_lruc_clear, sizeof(test_lruc_clear), "his address");
		intact_copy  = c;
		c.buckets_nb = 0;
		lruc_clear(&c);
		c         = intact_copy;
		c.node_nb = 0;
		lruc_clear(&c);
		c = intact_copy;
		lruc_clear(NULL);
		c = intact_copy;
		lruc_destroy(&c);
	}
	return;
}
NO_COV void test_lruc_remove(void) {
	t_lruc c; //, intact_copy;
	char  *k1 = "toto";
	int    k2 = 42;

	lruc_init(&c, 2, NULL);
	assert(lruc_put(&c, k1, strlen(k1), (void *)0xdead) == 0);
	assert(lruc_put(&c, &k2, sizeof(int), (void *)0xbafa) == 0);
	assert(lruc_get(&c, k1, strlen(k1)) == (void *)0xdead);
	assert(lruc_get(&c, &k2, sizeof(int)) == (void *)0xbafa);
	assert(c.MRU != NULL), assert(c.MRU->kv.key == &k2);
	assert(c.LRU != NULL), assert(c.LRU->kv.key == k1);
	assert(c.node_nb == 2);

	assert(lruc_remove(&c, test_lruc_remove, sizeof(test_lruc_remove)) == -1);
	assert(c.MRU != NULL), assert(c.MRU->kv.key == &k2);
	assert(c.LRU != NULL), assert(c.LRU->kv.key == k1);
	assert(c.node_nb == 2);

	assert(lruc_remove(&c, k1, strlen(k1)) == 0);
	assert(c.MRU != NULL), assert(c.MRU->kv.key == &k2);
	assert(c.LRU != NULL), assert(c.LRU->kv.key == &k2);
	assert(c.node_nb == 1);
	assert(lruc_get(&c, k1, strlen(k1)) == NULL);
	assert(lruc_remove(&c, &k2, sizeof(int)) == 0);

	// errors
	assert(c.MRU == NULL), assert(c.LRU == NULL);
	assert(c.node_nb == 0);
	assert(lruc_get(&c, &k2, sizeof(int)) == NULL);
	assert(lruc_remove(&c, &k2, sizeof(int)) != 0);
	assert(c.MRU == NULL), assert(c.LRU == NULL);
	assert(c.node_nb == 0);
	c.LRU = (void *)0xDEAD;
	assert(lruc_remove(&c, &k2, sizeof(int)) != 0);
	assert(c.MRU == NULL), assert(c.LRU == (void *)0xDEAD);
	assert(c.node_nb == 0);
	c.LRU = NULL;
	c.MRU = (void *)0xDEAD;
	assert(lruc_remove(&c, &k2, sizeof(int)) != 0);
	assert(c.MRU == (void *)0xDEAD), assert(c.LRU == NULL);
	assert(c.node_nb == 0);
	c.LRU = (void *)0xDEAD;
	assert(lruc_remove(&c, &k2, sizeof(int)) != 0);
	assert(c.MRU == (void *)0xDEAD), assert(c.LRU == (void *)0xDEAD);
	assert(c.node_nb == 0);
	c.MRU = NULL;
	c.LRU = NULL;
	assert(lruc_remove(&c, &k2, sizeof(int)) != 0);
	assert(c.MRU == NULL), assert(c.LRU == NULL);
	assert(c.node_nb == 0);

	assert(lruc_remove(NULL, &k2, sizeof(int)) != 0);
	lruc_destroy(&c);
	return;
}
