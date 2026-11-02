#include <stdlib.h>

#define C_MINI__TWRAP_IMPL
#include "cdsu_tests.h"
#include "dsuc_tests.h"
#include "rdsu_tests.h"

NO_COV int cdsu_main_tests(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create", )
	test_cdsu_new();
	test_cdsu_new_from_array();
	test_cdsu_init();
	test_cdsu_init_from_arrays();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Delete", )
	test_cdsu_clr();
	test_cdsu_destroy();
	test_cdsu_free();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Infos", )
	test_cdsu_max_depth();
	test_cdsu_size();
	test_cdsu_nb_components();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Grow", )
	test_cdsu_grow();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Links (union + compress)", )
	test_cdsu_strict_find();
	test_cdsu_find();
	test_cdsu_compress_all();
	test_cdsu_union();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Querry", )
	test_cdsu_validate();
	test_cdsu_is_related();
	test_cdsu_strict_is_related();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All cdsu tests completed successfully.", )
	return (EXIT_SUCCESS);
}
NO_COV int rdsu_main_tests(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create", )
	test_rdsu_new();
	test_rdsu_new_from_array();
	test_rdsu_init();
	test_rdsu_init_from_arrays();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Delete", )
	test_rdsu_clr();
	test_rdsu_destroy();
	test_rdsu_free();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Infos", )
	test_rdsu_max_depth();
	test_rdsu_size();
	test_rdsu_nb_components();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Grow", )
	test_rdsu_grow();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Links (union + compress)", )
	test_rdsu_find();
	test_rdsu_compress_all();
	test_rdsu_union();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Querry", )
	test_rdsu_validate();
	test_rdsu_is_related();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Rewind", )
	test_rdsu_save_state();
	test_rdsu_rewind();
	test_rdsu_rewind_to_state();
	test_rdsu_rewind_clear();
	test_rdsui_push_rewind();
	test_rdsui_log_reuse();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "All rdsu tests completed successfully.", )
	return (EXIT_SUCCESS);
}
NO_COV int conv_main_tests(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tCDSU -> RDSU", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_cdsu_new_from_rdsu();
	test_cdsu_init_from_rdsu();
	test_cdsu_to_rdsu();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tCDSU -> RDSU", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_rdsu_new_from_cdsu();
	test_rdsu_init_from_cdsu();
	test_rdsu_to_cdsu();
	return (EXIT_SUCCESS);
}

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tCDSU", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	cdsu_main_tests();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tRDSU", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	rdsu_main_tests();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tCONVERTIONS", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	conv_main_tests();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All dsu tests completed successfully.", )
	return (EXIT_SUCCESS);
}
