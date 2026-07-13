#include <assert.h>
#include <string.h>

#include "lru_cache_tests.h"

void blank(void *t) {
	(void)t;
}

// rms
void test_lruc_clear(void) {
	t_lruc c;
	char   empty_buff[sizeof(t_lruc_keynode *) * 32] = {0};

	{
		lruc_init(&c, 2, blank);
		lruc_put(&c, test_lruc_clear, sizeof(test_lruc_clear), "his address");
		lruc_get(&c, test_lruc_clear, sizeof(test_lruc_clear));
		lruc_clear(&c);

		assert(c.LRU == NULL);
		assert(c.MRU == NULL);
		assert(c.node_in_use == 0);
		assert(c.buckets_nb == 2);
		assert(c.dtor == blank);

		assert(memcmp(c.buckets, empty_buff, sizeof(t_lruc_keynode *) * 2) == 0);
		lruc_destroy(&c);
	}
	{ // test for low% fill
		lruc_init(&c, 32, NULL);
		lruc_put(&c, test_lruc_clear, sizeof(test_lruc_clear), "his address");
		lruc_clear(&c);

		assert(c.LRU == NULL);
		assert(c.MRU == NULL);
		assert(c.node_in_use == 0);
		assert(c.buckets_nb == 32);

		assert(memcmp(c.buckets, empty_buff, sizeof(t_lruc_keynode *) * (32)) == 0);
		lruc_destroy(&c);
	}
	return;
}
void test_lruc_remove(void) {
	t_lruc c;
	char  *k1 = "toto";
	int    k2 = 42;

	lruc_init(&c, 2, NULL);
	assert(lruc_put(&c, k1, strlen(k1), (void *)0xdead) == 0);
	assert(lruc_put(&c, &k2, sizeof(int), (void *)0xbafa) == 0);

	assert(lruc_get(&c, k1, strlen(k1)) == (void *)0xdead);
	assert(lruc_get(&c, &k2, sizeof(int)) == (void *)0xbafa);

	assert(lruc_remove(&c, test_lruc_remove, sizeof(test_lruc_remove)) == -1);
	assert(lruc_remove(&c, k1, strlen(k1)) == 0);
	assert(lruc_get(&c, k1, strlen(k1)) == NULL);
	assert(lruc_remove(NULL, &k2, sizeof(int)) != 0);

	lruc_destroy(&c);
	return;
}
