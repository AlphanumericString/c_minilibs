#include <stdlib.h>
#include <unistd.h>

#define C_MINI__TWRAP_IMPL
#include "lru_cache_tests.h"

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create", )
	test_lruc_create();
	test_lruc_init();
	test_lruc_destroy();
	test_lruc_free();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Put", )
	test_lruc_put_kv();
	test_lruc_put();
	test_lruc_put_ttl();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_lruc_get();
	test_lruc_get_lru();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Rm", )
	test_lruc_clear();
	test_lruc_remove();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Has", )
	test_lruc_has();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Expired", )
	test_lruc_isexpired();
	test_lruc_cleanup();
	test_lruc_setttl();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Info", )
	test_lruc_expired();
	test_lruc_size();
	test_lruc_hits();
	test_lruc_misses();
	test_lruc_stats();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Internals", )
	test_lruci_hp_pop();
	test_lruci_hp_pop_nth();
	test_lruci_keyinlist();
	test_lruci_node_eq();
	test_lruci_hsh_pop();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Foreach", )
	test_lruc_foreach();
	test_lruc_foreach_ru_ordered();
	test_lruc_foreach_ttl_ordered();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All LRU CACHE tests completed successfully.", )
	return (EXIT_SUCCESS);
}
