#define C_MINI__TWRAP_IMPL
#include "hp_tests.h"

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create");
	test_hp_create();
	test_hp_init();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Delete");
	test_hp_clear();
	test_hp_delete();
	test_hp_free();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Push");
	test_hp_min_push();
	test_hp_max_push();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Pop");
	test_hp_min_pop();
	test_hp_max_pop();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Rm");
	test_hp_max_rm();
	test_hp_min_rm();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Foreach");
	test_hp_foreach();
	test_hp_foreach_in();
	test_hp_foreach_pre();
	test_hp_foreach_post();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All hp tests completed successfully.");
}
