#include <stdlib.h>

#define C_MINI__TWRAP_IMPL
#include "rb_tests.h"

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create", )
	test_cm_rb_create();
	test_cm_rb_init();
	test_cm_rb_from_array();
	test_cm_rb_new();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Delete", )
	test_cm_rb_destroy();
	test_cm_rb_free();
	test_cm_rb_clr();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Querry", )
	test_cm_rb_full();
	test_cm_rb_empty();
	test_cm_rb_size();
	test_cm_rb_capacity();
	test_cm_rb_remaining_space();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Rm", )
	test_cm_rb_drop();
	test_cm_rb_pop();
	test_cm_rb_npop();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Peek", )
	test_cm_rb_peek();
	test_cm_rb_npeek();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Push", )
	test_cm_rb_push();
	test_cm_rb_npush();
	test_cm_rb_push_safe();
	test_cm_rb_npush_safe();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Foreach", )
	test_cm_rb_apply();
	test_cm_rb_naplly();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Filter", )
	test_cm_rb_filter();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Access", )
	test_cm_rb_at();
	test_cm_rb_nat();
	test_cm_rb_get();
	test_cm_rb_nget();
	test_cm_rb_getn();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Find", )
	test_cm_rb_find();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Map", )
	test_cm_rb_map();
	test_cm_rb_nmap();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Grow", )
	test_cm_rb_grow();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All RB tests completed successfully.", )
	return (EXIT_SUCCESS);
}
