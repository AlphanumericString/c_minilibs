#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lruc_tests.h"

static char *to_add = "zod_";

static bool stradd_cb(t_cm_keyvalue *value, void *u_val) {
	char *s;

	assert(value);
	assert(u_val == to_add);
	assert(value->value);
	s = calloc(1 + strlen(value->value) + strlen(u_val), 1);
	strlcat(s, u_val, strlen(value->value) + strlen(u_val) + 1);
	strlcat(s, value->value, strlen(value->value) + strlen(u_val) + 1);
	free(value->value);
	value->value = s;
	return (true);
}

static bool record_order_cb(t_cm_keyvalue *value, void *u_val) {
	const char **order = u_val;
	int          i     = 0;
	while (order[i] != NULL)
		i++;
	order[i] = value->key;
	return (true);
}

NO_COV void test_cm_lruc_foreach(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	t_cm_lruc   c;

	lruc_init(&c, 4, free);
	lruc_put(&c, k1, strlen(k1), strdup("1"));
	lruc_put(&c, k2, strlen(k2), strdup("2"));
	lruc_put(&c, k3, strlen(k3), strdup("3"));
	lruc_put(&c, k4, strlen(k4), strdup("4"));
	lruc_foreach(&c, stradd_cb, to_add);
	lruc_foreach(NULL, stradd_cb, to_add);
	lruc_foreach(&c, NULL, to_add);
	assert(strcmp(lruc_try_get(&c, k1, strlen(k1)), "zod_1") == 0);
	assert(strcmp(lruc_try_get(&c, k2, strlen(k2)), "zod_2") == 0);
	assert(strcmp(lruc_try_get(&c, k3, strlen(k3)), "zod_3") == 0);
	assert(strcmp(lruc_try_get(&c, k4, strlen(k4)), "zod_4") == 0);
	lruc_destroy(&c);
	return;
}
/**
 * \brief every entry must be yielded exactly once, including the ones sharing a
 * bucket (collision chains)
**/
NO_COV void test_cm_lruc_foreach_collisions(void) {
	t_cm_lruc   c;
	const char *keys[] = {"a", "e", "b", "f"};
	const char *seen[5] = {NULL};

	lruc_init(&c, 4, NULL);
	for (size_t i = 0; i < 4; i++)
		assert(lruc_put(&c, keys[i], 1, (void *)keys[i]) == true);
	// premise: two pairs share a bucket
	assert(lruci_hash_djb2(keys[0], 1) % c.buckets_nb ==
	       lruci_hash_djb2(keys[1], 1) % c.buckets_nb);
	assert(lruci_hash_djb2(keys[2], 1) % c.buckets_nb ==
	       lruci_hash_djb2(keys[3], 1) % c.buckets_nb);
	lruc_foreach(&c, record_order_cb, seen);
	assert(seen[4] == NULL);
	for (size_t i = 0; i < 4; i++) {
		size_t hits = 0;
		for (size_t j = 0; j < 4; j++)
			if (seen[j] == keys[i]) hits++;
		assert(hits == 1);
	}
	lruc_destroy(&c);
	return;
}
NO_COV void test_cm_lruc_foreach_ru_ordered(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	t_cm_lruc   c;
	const char *order[5] = {NULL};

	lruc_init(&c, 4, free);
	lruc_put(&c, k1, strlen(k1), strdup("1"));
	lruc_put(&c, k2, strlen(k2), strdup("2"));
	lruc_put(&c, k3, strlen(k3), strdup("3"));
	lruc_put(&c, k4, strlen(k4), strdup("4"));
	lruc_foreach_ru_ordered(&c, record_order_cb, order);
	assert(order[0] == k4 && order[1] == k3 && order[2] == k2 &&
	       order[3] == k1);
	assert(order[4] == NULL);
	lruc_foreach_ru_ordered(NULL, record_order_cb, order);
	lruc_foreach_ru_ordered(&c, NULL, order);
	lruc_destroy(&c);
	return;
}
