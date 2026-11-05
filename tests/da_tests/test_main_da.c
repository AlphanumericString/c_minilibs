#include <stdlib.h>

#define C_MINI__TWRAP_IMPL
#include "da_tests.h"

const int arr_3[]   = {1, 2, 3};
const int arr_5[]   = {1, 2, 3, 4, 5};
const int arr_2_5[] = {2, 3, 4, 5};

int NO_COV main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create", )
	test_cm_da_init();
	test_cm_da_ninit();
	test_cm_da_from_array();
	test_cm_da_new();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Dup", )
	test_cm_da_dup();
	test_cm_da_ndup();
	test_cm_da_dup_to();
	test_cm_da_ndup_to();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Destroy", )
	test_cm_da_clr();
	test_cm_da_destroy();
	test_cm_da_free();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Add", )
	test_cm_da_add();
	test_cm_da_cat_array();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Grow", )
	test_cm_da_grow();
	test_cm_da_grow_for_size();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Is", )
	test_cm_da_is_valid();
	test_cm_da_is_not_null();
	test_cm_da_is_empty();
	test_cm_da_is_full();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Shrink", )
	test_cm_da_shrink();
	test_cm_da_shrink_min();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_cm_da_at();
	test_cm_da_get();
	test_cm_da_nget();
	test_cm_da_array();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Set", )
	test_cm_da_set();
	test_cm_da_nset();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Swap", )
	test_cm_da_swap();
	test_cm_da_nswap();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Remove", )
	test_cm_da_rm_order();
	test_cm_da_rms_order();
	test_cm_da_rm();
	test_cm_da_rms();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Convert", )
	test_cm_da_to_array();
	test_cm_da_to_loc_array();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Apply", )
	test_cm_da_apply();
	test_cm_da_napply();
	test_cm_da_apply_it();
	test_cm_da_napply_it();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Fold", )
	test_cm_da_fold();
	test_cm_da_nfold();
	test_cm_da_reduce();
	test_cm_da_nreduce();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Filter", )
	test_cm_da_filter();
	test_cm_da_nfilter();
	test_cm_da_filter_out();
	test_cm_da_nfilter_out();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Advance", )
	test_cm_da_advance();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Reverse", )
	test_cm_da_reverse();
	test_cm_da_nreverse();
	test_cm_da_b_nreverse();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Find", )
	test_cm_da_find();
	test_cm_da_nfind();
	test_cm_da_find_get();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Sort", )
	test_cm_da_sort();
	test_cm_da_nsort();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All da tests completed successfully.", )
	return (EXIT_SUCCESS);
}
