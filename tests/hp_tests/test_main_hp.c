#define C_MINI__TWRAP_IMPL
#include "hp_tests.h"

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create", )
	test_cm_hp_create();
	test_cm_hp_init();
	test_cm_hp_new_from_array();
	test_cm_hp_init_from_array();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Delete", )
	test_cm_hp_clear();
	test_cm_hp_delete();
	test_cm_hp_free();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Is", )
	test_cm_hp_is_empty();
	test_cm_hp_is_full();
	test_cm_hp_is_valid();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_cm_hp_get_cmp();
	test_cm_hp_get_cap();
	test_cm_hp_get_current_size();
	test_cm_hp_get_elem_size();
	test_cm_hp_get_elems_array();
	test_cm_hp_at();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Push", )
	test_cm_hp_min_push();
	test_cm_hp_max_push();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Pop", )
	test_cm_hp_min_pop();
	test_cm_hp_max_pop();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Rm", )
	test_cm_hp_max_rm();
	test_cm_hp_min_rm();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Foreach", )
	test_cm_hp_foreach();
	test_cm_hp_foreach_in();
	test_cm_hp_foreach_pre();
	test_cm_hp_foreach_post();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All hp tests completed successfully.", )
}
