#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lru_cache_tests.h"

static char *to_add = "zod_";

static bool forward_stradd_cache(t_keyvalue *value, void *u_val) {
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

void test_lruc_foreach(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_lruc      c;

	v1 = strdup("toto_1");
	v2 = strdup("toto_2");
	v3 = strdup("toto_3");
	v4 = strdup("toto_4");
	lruc_init(&c, 4, free);
	lruc_put(&c, k1, strlen(k1), v1);
	lruc_put(&c, k2, strlen(k2), v2);
	lruc_put(&c, k3, strlen(k3), v3);
	lruc_put(&c, k4, strlen(k4), v4);
	lruc_foreach(&c, forward_stradd_cache, to_add);
	lruc_foreach(NULL, forward_stradd_cache, to_add);
	lruc_foreach(&c, NULL, to_add);
	assert(lruc_get(&c, k1, strlen(k1)) != NULL);
	assert(strcmp(lruc_get(&c, k1, strlen(k1)), "zod_toto_1") == 0);
	lruc_destroy(&c);
	return;
}

static bool record_order_cb(t_keyvalue *value, void *u_val) {
	const char **order = u_val;
	int          i     = 0;
	while (order[i] != NULL)
		i++;
	order[i] = value->key;
	return true;
}

void test_lruc_foreach_ru_ordered(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_lruc      c;
	const char *order[5] = {NULL};

	v1 = strdup("1");
	v2 = strdup("2");
	v3 = strdup("3");
	v4 = strdup("4");
	lruc_init(&c, 4, free);
	lruc_put(&c, k1, strlen(k1), v1);
	lruc_put(&c, k2, strlen(k2), v2);
	lruc_put(&c, k3, strlen(k3), v3);
	lruc_put(&c, k4, strlen(k4), v4);
	lruc_foreach_ru_ordered(&c, record_order_cb, order);
	assert(strcmp(order[0], k4) == 0);
	assert(strcmp(order[1], k3) == 0);
	assert(strcmp(order[2], k2) == 0);
	assert(strcmp(order[3], k1) == 0);
	assert(order[4] == NULL);
	lruc_destroy(&c);
}

void test_lruc_foreach_ttl_ordered(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_lruc      c;
	char       *order[5] = {NULL};

	v1 = strdup("1");
	v2 = strdup("2");
	v3 = strdup("3");
	v4 = strdup("4");
	lruc_init(&c, 4, free);
	lruc_put_ttl(&c, k1, strlen(k1), v1, 400);
	lruc_put_ttl(&c, k2, strlen(k2), v2, 100);
	lruc_put_ttl(&c, k3, strlen(k3), v3, 300);
	lruc_put_ttl(&c, k4, strlen(k4), v4, 200);
	lruc_foreach_ttl_ordered(&c, record_order_cb, order);
	assert(strcmp(order[0], k2) == 0);
	assert(strcmp(order[1], k4) == 0);
	assert(strcmp(order[2], k3) == 0);
	assert(strcmp(order[3], k1) == 0);
	assert(order[4] == NULL);
	lruc_destroy(&c);
}
