#include <stdio.h>
#include <stdlib.h>

#include "lru_cache_tests.h"

int main() {
	// // create
	// printf("create\n");
	// test_lruc_create();
	// test_lruc_init();
	// test_lruc_destroy();
	// // put
	// printf("put\n");
	// test_lruc_put_kv();
	// test_lruc_put();
	// test_lruc_put_ttl();
	// // get
	// printf("get\n");
	// test_lruc_get();
	// // rms
	// printf("rm\n");
	// test_lruc_clear();
	// test_lruc_remove();
	// // has
	// printf("has\n");
	// test_lruc_has();
	// // expired
	// printf("expired\n");
	// test_lruc_isexpired();
	// test_lruc_cleanup();
	// test_lruc_setttl();
	// infos
	printf("infos\n");
	test_lruc_expired();
	test_lruc_size();
	test_lruc_hits();
	test_lruc_misses();
	test_lruc_stats();
	// foreach
	// printf("foreach\n");
	// test_lruc_foreach();

	printf("All lru cache test passed\n");
	return (EXIT_SUCCESS);
}
