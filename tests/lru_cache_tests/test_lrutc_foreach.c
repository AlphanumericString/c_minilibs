#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lrutc_tests.h"

static char *to_add = "zod_";

static bool forward_stradd_cache(t_cm_keyvalue *value, void *u_val) {
	char *s;

	assert(value);
	assert(u_val == to_add);
	assert(value->value);
	s = calloc(sizeof(char), 1 + strlen(value->value) + strlen(u_val));
	strlcat(s, u_val, strlen(value->value) + strlen(u_val) + 1);
	strlcat(s, value->value, strlen(value->value) + strlen(u_val) + 1);
	free(value->value);
	value->value = s;
	return (true);
}

NO_COV void test_cm_lrutc_foreach(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_cm_lrutc   c;

	v1 = strdup("toto_1");
	v2 = strdup("toto_2");
	v3 = strdup("toto_3");
	v4 = strdup("toto_4");
	lrutc_init(&c, 4, free);
	lrutc_put(&c, k1, strlen(k1), v1);
	lrutc_put(&c, k2, strlen(k2), v2);
	lrutc_put(&c, k3, strlen(k3), v3);
	lrutc_put(&c, k4, strlen(k4), v4);
	lrutc_foreach(&c, forward_stradd_cache, to_add);
	lrutc_foreach(NULL, forward_stradd_cache, to_add);
	lrutc_foreach(&c, NULL, to_add);
	assert(lrutc_try_get(&c, k1, strlen(k1)) != NULL);
	assert(strcmp(lrutc_try_get(&c, k1, strlen(k1)), "zod_toto_1") == 0);
	lrutc_destroy(&c);
	return;
}

static bool record_order_cb(t_cm_keyvalue *value, void *u_val) {
	const char **order = u_val;
	int          i     = 0;
	while (order[i] != NULL)
		i++;
	order[i] = value->key;
	return true;
}

NO_COV void test_cm_lrutc_foreach_ru_ordered(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_cm_lrutc   c;
	const char *order[5] = {NULL};

	v1 = strdup("1");
	v2 = strdup("2");
	v3 = strdup("3");
	v4 = strdup("4");
	lrutc_init(&c, 4, free);
	lrutc_put(&c, k1, strlen(k1), v1);
	lrutc_put(&c, k2, strlen(k2), v2);
	lrutc_put(&c, k3, strlen(k3), v3);
	lrutc_put(&c, k4, strlen(k4), v4);
	lrutc_foreach_ru_ordered(&c, record_order_cb, order);
	assert(strcmp(order[0], k4) == 0);
	assert(strcmp(order[1], k3) == 0);
	assert(strcmp(order[2], k2) == 0);
	assert(strcmp(order[3], k1) == 0);
	assert(order[4] == NULL);
	lrutc_foreach_ru_ordered(NULL, record_order_cb, order);
	lrutc_foreach_ru_ordered(&c, NULL, order);
	lrutc_destroy(&c);
}

NO_COV void test_cm_lrutc_foreach_ttl_ordered(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_cm_lrutc   c;
	char       *order[5] = {NULL};

	v1 = strdup("1");
	v2 = strdup("2");
	v3 = strdup("3");
	v4 = strdup("4");
	lrutc_init(&c, 4, free);
	lrutc_put_ttl(&c, k1, strlen(k1), v1, LRUC_MS_TO_NS(400));
	lrutc_put_ttl(&c, k2, strlen(k2), v2, LRUC_MS_TO_NS(100));
	lrutc_put_ttl(&c, k3, strlen(k3), v3, LRUC_MS_TO_NS(300));
	lrutc_put_ttl(&c, k4, strlen(k4), v4, LRUC_MS_TO_NS(200));
	lrutc_foreach_ttl_ordered(&c, record_order_cb, order);
	lrutc_foreach_ttl_ordered(&c, NULL, order);
	lrutc_foreach_ttl_ordered(NULL, record_order_cb, order);
	assert(strcmp(order[0], k2) == 0);
	assert(strcmp(order[1], k4) == 0);
	assert(strcmp(order[2], k3) == 0);
	assert(strcmp(order[3], k1) == 0);
	assert(order[4] == NULL);
	lrutc_destroy(&c);
}

/**
 * \brief every entry must be yielded exactly once, including the ones sharing a
 * bucket (collision chains), which a bucket-head walk visits twice while
 * skipping the rest of the chain
**/
NO_COV void test_cm_lrutc_foreach_collisions(void) {
	t_cm_lrutc   c;
	const char *keys[]  = {"a", "e", "b", "f"};
	const char *seen[5] = {NULL};

	lrutc_init(&c, 4, NULL);
	for (size_t i = 0; i < 4; i++)
		assert(lrutc_put(&c, keys[i], 1, (void *)keys[i]) == true);
	// premise: two pairs share a bucket, so each chain holds two nodes
	assert(lrutci_hash_djb2(keys[0], 1) % c.buckets_nb ==
	       lrutci_hash_djb2(keys[1], 1) % c.buckets_nb);
	assert(lrutci_hash_djb2(keys[2], 1) % c.buckets_nb ==
	       lrutci_hash_djb2(keys[3], 1) % c.buckets_nb);
	lrutc_foreach(&c, record_order_cb, seen);
	assert(seen[4] == NULL);
	for (size_t i = 0; i < 4; i++) {
		size_t hits = 0;
		for (size_t j = 0; j < 4; j++)
			if (seen[j] == keys[i]) hits++;
		assert(hits == 1);
	}
	lrutc_destroy(&c);
	return;
}
