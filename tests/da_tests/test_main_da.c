#include <stdlib.h>

#define C_MINI__TWRAP_IMPL
#include "da_tests.h"

const int arr_3[]   = {1, 2, 3};
const int arr_5[]   = {1, 2, 3, 4, 5};
const int arr_2_5[] = {2, 3, 4, 5};

int NO_COV main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create")
	test_da_init();
	test_da_ninit();
	test_da_from_array();
	test_da_new();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Dup")
	test_da_dup();
	test_da_ndup();
	test_da_dup_to();
	test_da_ndup_to();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Destroy")
	test_da_clr();
	test_da_destroy();
	test_da_free();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Add")
	test_da_add();
	test_da_cat_array();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Grow")
	test_da_grow();
	test_da_grow_for_size();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Is")
	test_da_is_valid();
	test_da_is_not_null();
	test_da_is_empty();
	test_da_is_full();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Shrink")
	test_da_shrink();
	test_da_shrink_min();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get")
	test_da_at();
	test_da_get();
	test_da_nget();
	test_da_array();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Set")
	test_da_set();
	test_da_nset();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Swap")
	test_da_swap();
	test_da_nswap();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Remove")
	test_da_rm_order();
	test_da_rms_order();
	test_da_rm();
	test_da_rms();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Convert")
	test_da_to_array();
	test_da_to_loc_array();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Apply")
	test_da_apply();
	test_da_napply();
	test_da_apply_it();
	test_da_napply_it();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Fold")
	test_da_fold();
	test_da_nfold();
	test_da_reduce();
	test_da_nreduce();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Filter")
	test_da_filter();
	test_da_nfilter();
	test_da_filter_out();
	test_da_nfilter_out();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Advance")
	test_da_advance();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Reverse")
	test_da_reverse();
	test_da_nreverse();
	test_da_b_nreverse();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Find")
	test_da_find();
	test_da_nfind();
	test_da_find_get();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "")
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Sort")
	test_da_sort();
	test_da_nsort();

	CM_DBG("nb_alloc : %zu", nb_alloc);
	CM_LOG(C_MINI__LEVEL_ALWAYS, "All da tests completed successfully.");
	return (EXIT_SUCCESS);
}
