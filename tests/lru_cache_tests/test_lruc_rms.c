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
		cache_init(&c, 2, blank);
		cache_put(&c, test_lruc_clear, sizeof(test_lruc_clear), "his address");
		cache_get(&c, test_lruc_clear, sizeof(test_lruc_clear));
		cache_clear(&c);

		assert(c.LRU == NULL);
		assert(c.MRU == NULL);
		assert(c.node_in_use == 0);
		assert(c.buckets_nb == 2);
		assert(c.dtor == blank);

		assert(memcmp(c.buckets, empty_buff, sizeof(t_lruc_keynode *) * 2) == 0);
		cache_destroy(&c);
	}
	{ // test for low% fill
		cache_init(&c, 32, NULL);
		cache_put(&c, test_lruc_clear, sizeof(test_lruc_clear), "his address");
		cache_clear(&c);

		assert(c.LRU == NULL);
		assert(c.MRU == NULL);
		assert(c.node_in_use == 0);
		assert(c.buckets_nb == 32);

		assert(memcmp(c.buckets, empty_buff, sizeof(t_lruc_keynode *) * (32)) == 0);
		cache_destroy(&c);
	}
	return;
}
void test_lruc_remove(void) {
	t_lruc c;
	char  *k1 = "toto";
	int    k2 = 42;

	cache_init(&c, 2, NULL);
	assert(cache_put(&c, k1, strlen(k1), (void *)0xdead) == 0);
	assert(cache_put(&c, &k2, sizeof(int), (void *)0xbafa) == 0);

	assert(cache_get(&c, k1, strlen(k1)) == (void *)0xdead);
	assert(cache_get(&c, &k2, sizeof(int)) == (void *)0xbafa);

	assert(cache_remove(&c, test_lruc_remove, sizeof(test_lruc_remove)) == -1);
	assert(cache_remove(&c, k1, strlen(k1)) == 0);
	assert(cache_get(&c, k1, strlen(k1)) == NULL);
	assert(cache_remove(NULL, &k2, sizeof(int)) != 0);

	cache_destroy(&c);
	return;
}
