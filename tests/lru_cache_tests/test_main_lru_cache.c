#include <stdio.h>
#include <stdlib.h>

#include "lru_cache_tests.h"

int main() {
	// create
	test_lruc_create();
	test_lruc_init();
	test_lruc_destroy();
	// put
	test_lruc_put_kv();
	test_lruc_put();
	test_lruc_put_ttl();
	// get
	test_lruc_get();
	// rms
	test_lruc_clear();
	test_lruc_remove();
	// has
	test_lruc_has();
	// expired
	test_lruc_isexpired();
	test_lruc_cleanup();
	test_lruc_setttl();
	// infos
	test_lruc_size();
	test_lruc_hits();
	test_lruc_misses();
	test_lruc_expired();
	test_lruc_stats();
	// foreach
	test_lruc_foreach();

	printf("All lru cache test passed\n");
	return (EXIT_SUCCESS);
}
