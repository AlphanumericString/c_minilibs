#include <assert.h>
#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache_tests.h"

NO_COV void test_cm_lrutc_isexpired(void) {
	t_cm_lrutc   c;
	const char *k1 = "zod";
	void       *v1;

	lrutc_init(&c, 4, free);
	lrutc_setttl(&c, LRUC_MS_TO_NS(200));
	assert(lrutc_has(&c, k1, strlen(k1)) == false);
	assert(lrutc_isexpired(&c, k1, strlen(k1)) == false);
	v1 = strdup("should not leak");
	lrutc_put(&c, k1, strlen(k1), v1);
	assert(lrutc_has(&c, k1, strlen(k1)) == true);
	assert(lrutc_isexpired(&c, k1, strlen(k1)) == false);
	lrutc_test_sleep_ms(1000);
	assert(lrutc_has(&c, k1, strlen(k1)) == true);
	assert(lrutc_isexpired(&c, k1, strlen(k1)) == true);
	lrutc_setttl(&c, C_MINI__LRUTC_TTL_DFLT);
	assert(lrutc_isexpired(&c, k1, strlen(k1)) == false);

	assert(lrutc_isexpired(NULL, k1, strlen(k1)) == false);
	clock_set_call_limit(0);
	lrutc_setttl(&c, LRUC_MS_TO_NS(200));
	assert(lrutc_isexpired(&c, k1, strlen(k1)) == false);
	lrutc_setttl(&c, C_MINI__LRUTC_TTL_DFLT);
	assert(lrutc_isexpired(&c, k1, strlen(k1)) == false);
	clock_set_call_limit(CLOCK_DFT_LIMIT);

	lrutc_destroy(&c);
	return;
}
NO_COV void test_cm_lrutc_cleanup(void) {
	t_cm_lrutc   c, intact_copy;
	const char *key1 = "abc", *key2 = "def", *key3 = "ghi", *key4 = "jkl";
	void       *v1, *v2, *v3, *v4;

	lrutc_init(&c, 4, free);
	lrutc_setttl(&c, LRUC_MS_TO_NS(200));
	lrutc_put(&c, key1, strlen(key1), strdup("value_1"));
	lrutc_put(&c, key2, strlen(key2), strdup("value_2"));
	lrutc_put(&c, key3, strlen(key3), strdup("value_3"));
	lrutc_put(&c, key4, strlen(key4), strdup("value_4"));
	lrutc_test_sleep_ms(300);
	assert(lrutc_has(&c, key1, strlen(key1)) == true);
	assert(lrutc_has(&c, key2, strlen(key2)) == true);
	assert(lrutc_has(&c, key3, strlen(key3)) == true);
	assert(lrutc_has(&c, key4, strlen(key4)) == true);
	assert(lrutc_isexpired(&c, key1, strlen(key1)) == true);
	assert(lrutc_isexpired(&c, key2, strlen(key2)) == true);
	assert(lrutc_isexpired(&c, key3, strlen(key3)) == true);
	assert(lrutc_isexpired(&c, key4, strlen(key4)) == true);
	lrutc_cleanup(&c);
	assert(lrutc_has(&c, key1, strlen(key1)) == false);
	assert(lrutc_has(&c, key2, strlen(key2)) == false);
	assert(lrutc_has(&c, key3, strlen(key3)) == false);
	assert(lrutc_has(&c, key4, strlen(key4)) == false);
	assert(lrutc_expired(&c) == 4);
	c.dtor = NULL;
	v1     = strdup("value_1");
	v2     = strdup("value_2");
	v3     = strdup("value_3");
	v4     = strdup("value_4");

	lrutc_put(&c, key1, strlen(key1), v1);
	lrutc_put(&c, key2, strlen(key2), v2);
	lrutc_put(&c, key3, strlen(key3), v3);
	lrutc_put(&c, key4, strlen(key4), v4);
	lrutc_test_sleep_ms(300);
	assert(lrutc_isexpired(&c, key1, strlen(key1)) == true);
	assert(lrutc_isexpired(&c, key2, strlen(key2)) == true);
	assert(lrutc_isexpired(&c, key3, strlen(key3)) == true);
	assert(lrutc_isexpired(&c, key4, strlen(key4)) == true);
	clock_set_call_limit(0);
	lrutc_cleanup(&c);
	assert(lrutc_has(&c, key1, strlen(key1)) == true);
	assert(lrutc_has(&c, key2, strlen(key2)) == true);
	assert(lrutc_has(&c, key3, strlen(key3)) == true);
	assert(lrutc_has(&c, key4, strlen(key4)) == true);
	clock_set_call_limit(CLOCK_DFT_LIMIT);
	lrutc_cleanup(&c);
	assert(lrutc_has(&c, key1, strlen(key1)) == false);
	assert(lrutc_has(&c, key2, strlen(key2)) == false);
	assert(lrutc_has(&c, key3, strlen(key3)) == false);
	assert(lrutc_has(&c, key4, strlen(key4)) == false);
	assert(lrutc_expired(&c) == 8);
	free(v1);
	free(v2);
	free(v3);
	free(v4);
	intact_copy = c;

	c.node_nb = 0;
	lrutc_cleanup(&c);
	c         = intact_copy;
	c.minHeap = NULL;
	lrutc_cleanup(&c);
	c = intact_copy;
	lrutc_cleanup(NULL);

	lrutc_destroy(&c);
	return;
}
NO_COV void test_cm_lrutc_setttl(void) {
	t_cm_lrutc cache;

	lrutc_init(&cache, 4, free);
	assert(cache.ns_gap == C_MINI__LRUTC_TTL_DFLT);
	lrutc_setttl(&cache, LRUC_MS_TO_NS(200));
	assert(cache.ns_gap == LRUC_MS_TO_NS(200));

	// retroactive: existing entries get the new ttl
	lrutc_put(&cache, "k", 1, strdup("v"));
	assert(cache.MRU != NULL);
	assert(cache.MRU->ns_gap == LRUC_MS_TO_NS(200));
	lrutc_setttl(&cache, LRUC_MS_TO_NS(3000));
	assert(cache.ns_gap == LRUC_MS_TO_NS(3000));
	assert(cache.MRU->ns_gap == LRUC_MS_TO_NS(3000));
	lrutc_setttl(&cache, C_MINI__LRUTC_TTL_DFLT);
	assert(cache.ns_gap == C_MINI__LRUTC_TTL_DFLT);
	assert(cache.MRU->ns_gap == C_MINI__LRUTC_TTL_DFLT);

	lrutc_setttl(NULL, LRUC_MS_TO_NS(200));

	lrutc_destroy(&cache);
	return;
}
