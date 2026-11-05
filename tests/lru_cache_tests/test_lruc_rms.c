#include <assert.h>
#include <string.h>

#include "lru_cache_tests.h"

NO_COV static void blank(void *t) {
	(void)t;
}

NO_COV void test_cm_lruc_clear_reserved(void) {
	t_cm_lruc c;

	lruc_init(&c, 2, blank);
	lruc_put(&c, (void *)&blank, sizeof(void *), "a value");
	lruc_remove(&c, (void *)&blank, sizeof(void *));
	assert(c.reserved_nodes_nb == 1);
	assert(lruc_clear_reserved(&c) == true);
	assert(c.reserved_nodes_nb == 0);
	assert(lruc_clear_reserved(&c) == true);
	assert(c.reserved_nodes_nb == 0);
	assert(lruc_clear_reserved(NULL) == false);
	lruc_destroy(&c);
};

NO_COV void test_cm_lruc_clear(void) {
	t_cm_lruc c, intact_copy;
	char      empty_buff[sizeof(t_cm_lruc_keynode *) * 32] = {0};

	{ // lru 50% full
		lruc_init(&c, 2, blank);
		lruc_put(&c, (void *)&test_cm_lruc_clear, sizeof(void *),
		         "his address");
		lruc_get(&c, (void *)&test_cm_lruc_clear, sizeof(void *));
		lruc_clear(&c);
		assert(c.LRU == NULL);
		assert(c.MRU == NULL);
		assert(c.node_nb == 0);
		assert(c.buckets_nb == 2);
		assert(c.reserved_nodes_nb == 0);
		assert(c.dtor == blank);
		assert(!memcmp(c.buckets, empty_buff, sizeof(t_cm_lruc_keynode *) * 2));
		lruc_destroy(&c);
	}
	{ // lru 100% full
		lruc_init(&c, 2, blank);
		lruc_put(&c, (void *)&test_cm_lruc_clear, sizeof(void *),
		         "his address");
		lruc_put(&c, (void *)&blank, sizeof(void *), "value");
		lruc_clear(&c);
		assert(c.LRU == NULL);
		assert(c.MRU == NULL);
		assert(c.node_nb == 0);
		assert(c.reserved_nodes_nb == 0);
		assert(c.buckets_nb == 2);
		assert(c.dtor == blank);
		assert(!memcmp(c.buckets, empty_buff, sizeof(t_cm_lruc_keynode *) * 2));
		lruc_destroy(&c);
	}
	{ // lru with large holes
		lruc_init(&c, 32, NULL);
		lruc_put(&c, (void *)&test_cm_lruc_clear, sizeof(void *),
		         "his address");
		lruc_clear(&c);
		assert(c.LRU == NULL);
		assert(c.MRU == NULL);
		assert(c.node_nb == 0);
		assert(c.reserved_nodes_nb == 0);
		assert(c.buckets_nb == 32);
		assert(
		    !memcmp(c.buckets, empty_buff, sizeof(t_cm_lruc_keynode *) * (32)));
		lruc_destroy(&c);
	}
	{ // errors
		lruc_init(&c, 32, NULL);
		lruc_put(&c, (void *)&test_cm_lruc_clear, sizeof(void *),
		         "his address");
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
NO_COV void test_cm_lruc_clear_to_reserved(void) {
	t_cm_lruc          c, intact_copy;
	char               empty_buff[sizeof(t_cm_lruc_keynode *) * 32] = {0};
	t_cm_lruc_keynode *kn;

	{ // lru 50% full
		lruc_init(&c, 2, blank);
		lruc_put(&c, (void *)&test_cm_lruc_clear, sizeof(void *),
		         "his address");
		assert(c.node_nb == 1);
		lruc_clear_to_reserved(&c);
		assert(c.LRU == NULL);
		assert(c.MRU == NULL);
		assert(c.node_nb == 0);
		assert(c.buckets_nb == 2);
		assert(c.dtor == blank);
		assert(!memcmp(c.buckets, empty_buff, sizeof(t_cm_lruc_keynode *) * 2));
		assert(c.reserved_nodes_nb == 1);
		assert(c.reserved_nodes != NULL);
		assert(c.reserved_nodes->lru_next == NULL);
		lruc_destroy(&c);
	}
	{ // lru 100% full
		lruc_init(&c, 2, blank);
		lruc_put(&c, (void *)&test_cm_lruc_clear, sizeof(void *),
		         "his address");
		lruc_put(&c, (void *)&blank, sizeof(void *), "value");
		assert(c.node_nb == 2);
		lruc_clear_to_reserved(&c);
		assert(c.LRU == NULL);
		assert(c.MRU == NULL);
		assert(c.node_nb == 0);
		assert(c.buckets_nb == 2);
		assert(c.dtor == blank);
		assert(!memcmp(c.buckets, empty_buff, sizeof(t_cm_lruc_keynode *) * 2));
		assert(c.reserved_nodes_nb == 2);
		assert(c.reserved_nodes != NULL);
		assert(c.reserved_nodes->lru_next != NULL);
		lruc_destroy(&c);
	}
	{ // lru with large holes
		lruc_init(&c, 32, NULL);
		lruc_put(&c, (void *)&test_cm_lruc_clear, sizeof(void *),
		         "his address");
		kn = c.MRU;
		lruc_clear_to_reserved(&c);
		assert(c.LRU == NULL);
		assert(c.MRU == NULL);
		assert(c.node_nb == 0);
		assert(c.buckets_nb == 32);
		assert(
		    !memcmp(c.buckets, empty_buff, sizeof(t_cm_lruc_keynode *) * (32)));
		assert(c.reserved_nodes_nb == 1);
		assert(c.reserved_nodes != NULL);
		assert(c.reserved_nodes == kn);
		lruc_destroy(&c);
	}
	{ // errors
		lruc_init(&c, 32, NULL);
		lruc_put(&c, (void *)&test_cm_lruc_clear, sizeof(void *),
		         "his address");
		intact_copy  = c;
		c.buckets_nb = 0;
		lruc_clear_to_reserved(&c);
		c         = intact_copy;
		c.node_nb = 0;
		lruc_clear_to_reserved(&c);
		c = intact_copy;
		lruc_clear_to_reserved(NULL);
		c = intact_copy;
		lruc_destroy(&c);
	}
	return;
}
NO_COV void test_cm_lruc_remove(void) {
	t_cm_lruc c; //, intact_copy;
	char     *k1 = "toto";
	int       k2 = 42;

	lruc_init(&c, 2, NULL);
	assert(lruc_put(&c, k1, strlen(k1), (void *)0xdead) == true);
	assert(lruc_put(&c, (void *)&k2, sizeof(int), (void *)0xbafa) == true);
	assert(lruc_get(&c, k1, strlen(k1)) == (void *)0xdead);
	assert(lruc_get(&c, (void *)&k2, sizeof(int)) == (void *)0xbafa);
	assert(c.MRU != NULL), assert(c.MRU->kv.key == &k2);
	assert(c.LRU != NULL), assert(c.LRU->kv.key == k1);
	assert(c.node_nb == 2);

	assert(lruc_remove(&c, (void *)&test_cm_lruc_remove, sizeof(void *)) == -1);
	lruc_clear_reserved(&c);
	assert(c.MRU != NULL), assert(c.MRU->kv.key == &k2);
	assert(c.LRU != NULL), assert(c.LRU->kv.key == k1);
	assert(c.node_nb == 2);

	assert(lruc_remove(&c, k1, strlen(k1)) == 0);
	lruc_clear_reserved(&c);
	assert(c.MRU != NULL), assert(c.MRU->kv.key == &k2);
	assert(c.LRU != NULL), assert(c.LRU->kv.key == &k2);
	assert(c.node_nb == 1);
	assert(lruc_get(&c, k1, strlen(k1)) == NULL);
	assert(lruc_remove(&c, &k2, sizeof(int)) == 0);
	lruc_clear_reserved(&c);
	assert(c.MRU == NULL), assert(c.LRU == NULL);
	assert(c.node_nb == 0);
	assert(lruc_get(&c, &k2, sizeof(int)) == NULL);

	// errors
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
