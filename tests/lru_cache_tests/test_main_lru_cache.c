#include <stdlib.h>
#include <unistd.h>

#define C_MINI__TWRAP_IMPL
#include "lru_cache_tests.h"

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create", )
	test_cm_lrutc_create();
	test_cm_lrutc_init();
	test_cm_lrutc_destroy();
	test_cm_lrutc_free();
	test_cm_lrutc_reserve();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Put", )
	test_cm_lrutc_put_kv();
	test_cm_lrutc_put();
	test_cm_lrutc_put_ttl();
	test_cm_lrutc_put_evict_lru_bucket();
	test_cm_lrutc_put_evict_ttl_bucket();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_cm_lrutc_get();
	test_cm_lrutc_getttl();
	test_cm_lrutc_try_get();
	test_cm_lrutc_get_lru();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Rm", )
	test_cm_lrutc_clear();
	test_cm_lrutc_clear_reserved();
	test_cm_lrutc_clear_to_reserved();
	test_cm_lrutc_remove();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Has", )
	test_cm_lrutc_has();
	test_cm_lrutc_has_ttl();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Expired", )
	test_cm_lrutc_isexpired();
	test_cm_lrutc_cleanup();
	test_cm_lrutc_setttl();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Info", )
	test_cm_lrutc_expired();
	test_cm_lrutc_size();
	test_cm_lrutc_hits();
	test_cm_lrutc_misses();
	test_cm_lrutc_stats();
	test_cm_lrutc_is_valid();
	test_cm_lrutc_reserved_nb();
	test_cm_lrutc_has_reserved();
	test_cm_lrutc_max_reserve();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Internals", )
	test_cm_lrutci_hp_pop();
	test_cm_lrutci_hp_pop_sift_right();
	test_cm_lrutci_hp_pop_nth();
	test_cm_lrutci_hp_rebuild();
	test_cm_lrutci_keyinhash();
	test_cm_lrutci_keyinlist();
	test_cm_lrutci_node_eq();
	test_cm_lrutci_expiry();
	test_cm_lrutci_expired();
	test_cm_lrutci_hsh_pop();
	test_cm_lrutci_validate();
	test_cm_lrutci_unlink_untracked();
	test_cm_lrutci_rsvrd_push();
	test_cm_lrutci_rsvrd_pop();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Foreach", )
	test_cm_lrutc_foreach();
	test_cm_lrutc_foreach_collisions();
	test_cm_lrutc_foreach_ru_ordered();
	test_cm_lrutc_foreach_ttl_ordered();

	CM_LOG(C_MINI__LEVEL_ALWAYS,
	       "All LRU CACHE tests completed successfully.", )
	return (EXIT_SUCCESS);
}
