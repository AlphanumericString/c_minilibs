#include "da_tests.h"

const int arr_3[]   = {1, 2, 3};
const int arr_5[]   = {1, 2, 3, 4, 5};
const int arr_2_5[] = {2, 3, 4, 5};

int main(void) {
	test_da_init();
	test_da_ninit();
	test_da_from_array();
	test_da_new();
	test_da_dup();
	test_da_ndup();
	test_da_dup_to();
	test_da_ndup_to();
	test_da_clr();
	test_da_destroy();
	test_da_add();
	test_da_cat_array();
	test_da_grow();
	test_da_grow_for_size();
	test_da_shrink();
	test_da_shrink_min();
	test_da_at();
	test_da_get();
	test_da_nget();
	test_da_array();
	test_da_set();
	test_da_nset();
	test_da_swap();
	test_da_nswap();
	test_da_rm_order();
	test_da_rms_order();
	test_da_rm();
	test_da_rms();
	test_da_to_array();
	test_da_to_loc_array();
	test_da_apply();
	test_da_napply();
	test_da_apply_it();
	test_da_napply_it();
	test_da_fold();
	test_da_nfold();
	test_da_reduce();
	test_da_nreduce();
	test_da_filter();
	test_da_nfilter();
	test_da_filter_out();
	test_da_nfilter_out();
	test_da_advance();
	test_da_reverse();
	test_da_nreverse();
	test_da_b_nreverse();
	test_da_find();
	test_da_nfind();
	test_da_find_get();
	test_da_sort();
	test_da_nsort();

	printf("All da tests completed successfully.\n");
	return (EXIT_SUCCESS);
}
