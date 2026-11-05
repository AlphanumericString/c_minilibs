#include <stdlib.h>

#define C_MINI__TWRAP_IMPL
#include "cdsu_tests.h"
#include "dsuc_tests.h"
#include "rdsu_tests.h"

NO_COV int cdsu_main_tests(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create", )
	test_cm_cdsu_new();
	test_cm_cdsu_new_from_array();
	test_cm_cdsu_init();
	test_cm_cdsu_init_from_arrays();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Delete", )
	test_cm_cdsu_clr();
	test_cm_cdsu_destroy();
	test_cm_cdsu_free();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Infos", )
	test_cm_cdsu_max_depth();
	test_cm_cdsu_size();
	test_cm_cdsu_nb_components();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Grow", )
	test_cm_cdsu_grow();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Links (union + compress)", )
	test_cm_cdsu_strict_find();
	test_cm_cdsu_find();
	test_cm_cdsu_compress_all();
	test_cm_cdsu_union();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Querry", )
	test_cm_cdsu_validate();
	test_cm_cdsu_is_related();
	test_cm_cdsu_strict_is_related();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All cdsu tests completed successfully.", )
	return (EXIT_SUCCESS);
}
NO_COV int rdsu_main_tests(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create", )
	test_cm_rdsu_new();
	test_cm_rdsu_new_from_array();
	test_cm_rdsu_init();
	test_cm_rdsu_init_from_arrays();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Delete", )
	test_cm_rdsu_clr();
	test_cm_rdsu_destroy();
	test_cm_rdsu_free();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Infos", )
	test_cm_rdsu_max_depth();
	test_cm_rdsu_size();
	test_cm_rdsu_nb_components();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Grow", )
	test_cm_rdsu_grow();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Links (union + compress)", )
	test_cm_rdsu_find();
	test_cm_rdsu_compress_all();
	test_cm_rdsu_union();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Querry", )
	test_cm_rdsu_validate();
	test_cm_rdsu_is_related();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Rewind", )
	test_cm_rdsu_save_state();
	test_cm_rdsu_rewind();
	test_cm_rdsu_rewind_to_state();
	test_cm_rdsu_rewind_clear();
	test_cm_rdsui_log_reuse();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "All rdsu tests completed successfully.", )
	return (EXIT_SUCCESS);
}
NO_COV int conv_main_tests(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tCDSU -> RDSU", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_cm_cdsu_new_from_rdsu();
	test_cm_cdsu_init_from_rdsu();
	test_cm_cdsu_to_rdsu();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tCDSU -> RDSU", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_cm_rdsu_new_from_cdsu();
	test_cm_rdsu_init_from_cdsu();
	test_cm_rdsu_to_cdsu();
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
