#include <stdlib.h>
#include <unistd.h>

#define C_MINI__TWRAP_IMPL
#include "lru_cache_tests.h"

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create", )
	test_cm_lruc_create();
	test_cm_lruc_init();
	test_cm_lruc_destroy();
	test_cm_lruc_free();
	test_cm_lruc_reserve();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Put", )
	test_cm_lruc_put_kv();
	test_cm_lruc_put();
	test_cm_lruc_put_ttl();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_cm_lruc_get();
	test_cm_lruc_getttl();
	test_cm_lruc_try_get();
	test_cm_lruc_get_lru();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Rm", )
	test_cm_lruc_clear();
	test_cm_lruc_clear_reserved();
	test_cm_lruc_clear_to_reserved();
	test_cm_lruc_remove();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Has", )
	test_cm_lruc_has();
	test_cm_lruc_has_ttl();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Expired", )
	test_cm_lruc_isexpired();
	test_cm_lruc_cleanup();
	test_cm_lruc_setttl();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Info", )
	test_cm_lruc_expired();
	test_cm_lruc_size();
	test_cm_lruc_hits();
	test_cm_lruc_misses();
	test_cm_lruc_stats();
	test_cm_lruc_is_valid();
	test_cm_lruc_reserved_nb();
	test_cm_lruc_has_reserved();
	test_cm_lruc_max_reserve();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Internals", )
	test_cm_lruci_hp_pop();
	test_cm_lruci_hp_pop_sift_right();
	test_cm_lruci_hp_pop_nth();
	test_cm_lruci_hp_rebuild();
	test_cm_lruci_keyinhash();
	test_cm_lruci_keyinlist();
	test_cm_lruci_node_eq();
	test_cm_lruci_expiry();
	test_cm_lruci_expired();
	test_cm_lruci_hsh_pop();
	test_cm_lruci_rsvrd_push();
	test_cm_lruci_rsvrd_pop();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Foreach", )
	test_cm_lruc_foreach();
	test_cm_lruc_foreach_ru_ordered();
	test_cm_lruc_foreach_ttl_ordered();

	CM_LOG(C_MINI__LEVEL_ALWAYS,
	       "All LRU CACHE tests completed successfully.", )
	return (EXIT_SUCCESS);
}
