#include <assert.h>
#include <lru_cache/least_recently_used_cache.h>
#include <stdlib.h>
#include <string.h>

#include "lru_cache_tests.h"

static char *to_add = "zod_";

static void forward_stradd_cache(t_keyCache *kv, void *u_val)
{
	char *s;

	assert(kv);
	assert(u_val == to_add);
	assert(kv->value);
	s = kv->value;
	s = calloc(sizeof(char), 1 + strlen(kv->value) + strlen(u_val));
	strlcat(s, u_val, strlen(kv->value) + strlen(u_val) + 1);
	strlcat(s, kv->value, strlen(kv->value) + strlen(u_val) + 1);
	free(kv->value);
	kv->value = s;
}

// foreach
void test_lruc_foreach(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_lruc c;

	v1 = strdup("toto_1");
	v2 = strdup("toto_2");
	v3 = strdup("toto_3");
	v4 = strdup("toto_4");
	cache_init(&c, 4, free);
	cache_put(&c, k1, strlen(k1), v1);
	cache_put(&c, k2, strlen(k2), v2);
	cache_put(&c, k3, strlen(k3), v3);
	cache_put(&c, k4, strlen(k4), v4);
	cache_foreach(&c, forward_stradd_cache, to_add);
	cache_foreach(NULL, forward_stradd_cache, to_add);
	cache_foreach(&c, NULL, to_add);
	assert(cache_get(&c, k1, strlen(k1)) != NULL);
	assert(strcmp(cache_get(&c, k1, strlen(k1)), "zod_toto_1") == 0);

	cache_destroy(&c);
	return;
}
