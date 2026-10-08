#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "lru_cache_tests.h"

int lruc_tests(void) {
	LOG("");
	LOG("Create");
	test_lruc_create();
	test_lruc_init();
	test_lruc_destroy();
	test_lruc_free();
	LOG("");
	LOG("Put");
	test_lruc_put_kv();
	test_lruc_put();
	test_lruc_put_ttl();
	LOG("");
	LOG("Get");
	test_lruc_get();
	test_lruc_get_lru();
	LOG("");
	LOG("Rm");
	test_lruc_clear();
	test_lruc_remove();
	LOG("");
	LOG("Has");
	test_lruc_has();
	LOG("");
	LOG("Expired");
	test_lruc_isexpired();
	test_lruc_cleanup();
	test_lruc_setttl();
	LOG("");
	LOG("Info");
	test_lruc_expired();
	test_lruc_size();
	test_lruc_hits();
	test_lruc_misses();
	test_lruc_stats();
	LOG("");
	LOG("Internals");
	test_lruci_hp_pop();
	test_lruci_hp_pop_nth();
	test_lruci_keyinlist();
	test_lruci_node_eq();

	LOG("");
	LOG("Foreach");
	test_lruc_foreach();
	test_lruc_foreach_ru_ordered();
	test_lruc_foreach_ttl_ordered();

	printf("All lru cache test passed\n");
	return (EXIT_SUCCESS);
}

int main(void) {
	return (lruc_tests());
}
