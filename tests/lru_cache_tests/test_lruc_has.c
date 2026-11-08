#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "lru_cache_tests.h"

NO_COV void test_cm_lruc_has(void) {
	const char *k1 = "abc", *k2 = "def", *k3 = "ghi", *k4 = "jkl";
	void       *v1, *v2, *v3, *v4;
	t_cm_lruc   c, intact_copy;

	v1 = strdup("toto_1");
	v2 = strdup("toto_2");
	v3 = strdup("toto_3");
	v4 = strdup("toto_4");
	lruc_init(&c, 2, free);
	lruc_put(&c, k1, strlen(k1), v1);
	lruc_put(&c, k2, strlen(k2), v2);
	lruc_put(&c, k3, strlen(k3), v3);
	lruc_put(&c, k4, strlen(k4), v4);
	assert(lruc_has(&c, k1, strlen(k1)) == false);
	assert(lruc_has(&c, k2, strlen(k2)) == false);
	assert(lruc_has(&c, k3, strlen(k3)) != false);
	assert(lruc_has(&c, k4, strlen(k4)) != false);
	assert(lruc_has(&c, k3, strlen(k3)) == true);
	assert(lruc_has(&c, k4, strlen(k4)) == true);
	intact_copy = c;

	c.node_nb = 0;
	assert(lruc_has(&c, k4, strlen(k4)) == false);
	c = intact_copy;

	c.MRU = NULL;
	assert(lruc_has(&c, k4, strlen(k4)) == false);
	c = intact_copy;

	assert(lruc_has(NULL, k4, strlen(k4)) == false);
	lruc_destroy(&c);
	return;
}

NO_COV void test_cm_lruc_has_ttl(void) {
	const char *k1 = "abc", *k2 = "def";
	t_cm_lruc   c, intact_copy;

	lruc_init(&c, 2, free);

	// no ttl -> has_ttl behaves like has
	lruc_put(&c, k2, strlen(k2), strdup("toto_2"));
	assert(lruc_has_ttl(&c, k2, strlen(k2)) == true);

	// ttl set before insertion -> present and not expired
	lruc_setttl(&c, LRUC_MS_TO_NS(100));
	lruc_put(&c, k1, strlen(k1), strdup("toto_1"));
	assert(lruc_has_ttl(&c, k1, strlen(k1)) == true);

	usleep(150 * 1000);
	assert(lruc_has(&c, k1, strlen(k1)) == true);      // has ignores ttl
	assert(lruc_has_ttl(&c, k1, strlen(k1)) == false); // has_ttl honors it
	assert(lruc_has_ttl(&c, k2, strlen(k2)) == false); // retroactively ttl'd
	intact_copy = c;

	c.node_nb = 0;
	assert(lruc_has_ttl(&c, k1, strlen(k1)) == false);
	c = intact_copy;

	// clock failure with a ttl'd key -> false
	clock_set_call_limit(0);
	assert(lruc_has_ttl(&c, k1, strlen(k1)) == false);
	clock_set_call_limit(CLOCK_DFT_LIMIT);

	// unknown key
	assert(lruc_has_ttl(&c, "nope", 4) == false);

	assert(lruc_has_ttl(NULL, k1, strlen(k1)) == false);

	lruc_destroy(&c);
	return;
}
