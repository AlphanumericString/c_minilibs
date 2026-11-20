#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "lruc_tests.h"

NO_COV void test_cm_lruc_has(void) {
	t_cm_lruc   c;
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi";

	lruc_init(&c, 2, free);
	assert(lruc_has(&c, k1, strlen(k1)) == false); // empty but valid
	lruc_put(&c, k1, strlen(k1), strdup("1"));
	lruc_put(&c, k2, strlen(k2), strdup("2"));
	assert(lruc_has(&c, k1, strlen(k1)) == true);
	assert(lruc_has(&c, k2, strlen(k2)) == true);
	assert(lruc_has(&c, k3, strlen(k3)) == false);

	// a hit does not touch the recency order, has is a pure lookup
	assert(c.LRU->kv.key == k1);
	lruc_remove(&c, k1, strlen(k1));
	assert(lruc_has(&c, k1, strlen(k1)) == false);

	assert(lruc_has(NULL, k1, strlen(k1)) == false);
	lruc_destroy(&c);
	return;
}
