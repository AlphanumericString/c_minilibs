#include <stdlib.h>

#define C_MINI__TWRAP_IMPL
#include "lruc_tests.h"
#include "lrutc_tests.h"

NO_COV int lruc_main_tests(void) {
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
	test_cm_lruc_put_evict();
	test_cm_lruc_put_evict_bucket();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_cm_lruc_try_get();
	test_cm_lruc_get();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Rm", )
	test_cm_lruc_clear();
	test_cm_lruc_clear_reserved();
	test_cm_lruc_clear_to_reserved();
	test_cm_lruc_remove();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Has", )
	test_cm_lruc_has();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Info", )
	test_cm_lruc_size();
	test_cm_lruc_hits();
	test_cm_lruc_misses();
	test_cm_lruc_expired();
	test_cm_lruc_stats();
	test_cm_lruc_is_valid();
	test_cm_lruc_reserved_nb();
	test_cm_lruc_has_reserved();
	test_cm_lruc_max_reserve();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Internals", )
	test_cm_lruci_lru_push_pop();
	test_cm_lruci_hsh_pop();
	test_cm_lruci_rsvrd_push_pop();
	test_cm_lruci_keyinhash();
	test_cm_lruci_keyinlist();
	test_cm_lruci_node_eq();
	test_cm_lruci_unlink_node();
	test_cm_lruci_validate();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Foreach", )
	test_cm_lruc_foreach();
	test_cm_lruc_foreach_collisions();
	test_cm_lruc_foreach_ru_ordered();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Iterators", )
	test_cm_lruc_iter();
	test_cm_lruc_iter_promote_long();

	CM_LOG(C_MINI__LEVEL_ALWAYS,
	       "All LRU CACHE tests completed successfully.", )
	return (EXIT_SUCCESS);
}
NO_COV int lrutc_main_tests(void) {
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
	test_cm_lrutci_rsvrd_push();
	test_cm_lrutci_rsvrd_pop();
	test_cm_lrutci_validate();
	test_cm_lrutci_unlink_untracked();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Foreach", )
	test_cm_lrutc_foreach();
	test_cm_lrutc_foreach_collisions();
	test_cm_lrutc_foreach_ru_ordered();
	test_cm_lrutc_foreach_ttl_ordered();

	CM_LOG(C_MINI__LEVEL_ALWAYS,
	       "All LRU TIMED CACHE tests completed successfully.", )
	return (EXIT_SUCCESS);
}

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tLRUC", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	lruc_main_tests();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tLRUTC", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	lrutc_main_tests();

	CM_LOG(C_MINI__LEVEL_ALWAYS,
	       "All lru_cache tests completed successfully.", )
	return (EXIT_SUCCESS);
}
