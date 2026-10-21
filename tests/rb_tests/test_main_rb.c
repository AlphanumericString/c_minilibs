#include <stdlib.h>

#define C_MINI__TWRAP_IMPL
#include "rb_tests.h"

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create", )
	test_rb_create();
	test_rb_init();
	test_rb_from_array();
	test_rb_new();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Delete", )
	test_rb_destroy();
	test_rb_free();
	test_rb_clr();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Querry", )
	test_rb_full();
	test_rb_empty();
	test_rb_size();
	test_rb_capacity();
	test_rb_remaining_space();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Rm", )
	test_rb_drop();
	test_rb_pop();
	test_rb_npop();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Peek", )
	test_rb_peek();
	test_rb_npeek();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Push", )
	test_rb_push();
	test_rb_npush();
	test_rb_push_safe();
	test_rb_npush_safe();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Foreach", )
	test_rb_apply();
	test_rb_naplly();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Filter", )
	test_rb_filter();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Access", )
	test_rb_at();
	test_rb_nat();
	test_rb_get();
	test_rb_nget();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Find", )
	test_rb_find();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Map", )
	test_rb_map();
	test_rb_nmap();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Grow", )
	test_rb_grow();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All RB tests completed successfully.", )
	return (EXIT_SUCCESS);
}
