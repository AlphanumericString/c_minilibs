#include <assert.h>

#include "da_tests.h"

void test_da_filter(void) {
	int  arr[5] = {1, 2, 3, 4, 5};
	t_da da     = {.array = arr, .cap_e = 5, .nb_e = 5, .sz_e = sizeof(int)};
	int  bound;

	bound = 2;
	assert(da_filter(&da, cb_filter_strict_inf, &bound, cb_empty_deletor) ==
	       true);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 1);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 5);
	bound = 3;
	assert(da_filter(&da, cb_filter_strict_inf, &bound, cb_empty_deletor) ==
	       true);
	da_sort(&da, cb_cmp);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 2);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	da.nb_e = 1;
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int)); // 1-..-5
	assert(da_filter(&da, cb_filter_strict_inf, &bound, NULL) == true);
	assert(da.nb_e == 2);
	assert(da_filter(&da, NULL, &bound, cb_empty_deletor) == false);
	assert(da_filter(NULL, cb_filter_strict_inf, &bound, cb_empty_deletor) ==
	       false);
	da.nb_e = 0;
	assert(da_filter(&da, cb_filter_strict_inf, &bound, NULL) == false);
}
void test_da_nfilter(void) {
	int        arr[5] = {1, 2, 3, 4, 5};
	t_da       da = {.array = arr, .cap_e = 5, .nb_e = 5, .sz_e = sizeof(int)};
	int        bound       = 1;
	t_callback func_filter = cb_filter_strict_inf;
	const t_callback del   = cb_empty_deletor;

	assert(da_nfilter(&da, 4, cb_false, &bound, del) == true);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 1);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 5);
	bound = 3;
	assert(da_nfilter(&da, 4, func_filter, &bound, del) == true);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 2);
	assert(da_nfilter(&da, 0, func_filter, &bound, del) == true);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 2);
	assert(da_nfilter(NULL, 4, func_filter, &bound, del) == false);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 2);
	assert(da_nfilter(&da, 4, NULL, &bound, del) == false);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 2);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 5);
	assert(da_nfilter(&da, 4, cb_true, &bound, del) == true);
	assert(da.nb_e == 2);
	assert(da_nfilter(&da, 4, cb_false, &bound, del) == true);
	da.nb_e = 2;
	assert(da_nfilter(&da, 4, cb_false, &bound, NULL) == true);
	assert(da.nb_e == 0);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 0);
	assert(da_is_empty(&da) == true);
	assert(da_nfilter(&da, 1, cb_true, &bound, del) == false);
}
void test_da_filter_out(void) {
	t_da da;
	int  a     = 1;
	int  bound = 3;

	da_init(&da, sizeof(int));
	da_add(&da, &a);
	da_filter_out(&da, cb_filter_strict_inf, &bound, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 0);
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	da_filter_out(&da, cb_filter_strict_inf, &bound, NULL);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 3);
	da_sort(&da, cb_cmp);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 5);

	da_filter_out(NULL, cb_filter_strict_inf, &bound, cb_empty_deletor);
	da_filter_out(&da, NULL, &bound, cb_empty_deletor);
	da_destroy(&da);
	t_da bad_da = {.array = (void *)0xDEAD, .cap_e = 5, .nb_e = 0, .sz_e = 4};
	da_filter_out(&bad_da, cb_filter_strict_inf, &bound, cb_empty_deletor);
}
void test_da_nfilter_out(void) {
	t_da da;
	int  a     = 1;
	int  bound = 3;

	da_init(&da, sizeof(int));
	da_add(&da, &a);
	da_nfilter_out(&da, 1, cb_true, &bound, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 0);
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	da_nfilter_out(&da, 1, cb_filter_strict_inf, &bound, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 4);
	da_sort(&da, cb_cmp);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	assert(*(int *)da_at(&da, 3) == 5);
	da_nfilter_out(&da, 1, NULL, &bound, cb_empty_deletor);
	da_nfilter_out(NULL, 1, cb_filter_strict_inf, &bound, cb_empty_deletor);
	da_nfilter_out(&da, 0, cb_filter_strict_inf, &bound, cb_empty_deletor);
	da_nfilter_out(&da, -1, cb_filter_strict_inf, &bound, NULL);
	da_destroy(&da);
	t_da bad_da = {.array = (void *)0xDEAD, .cap_e = 5, .nb_e = 0, .sz_e = 4};
	da_nfilter_out(&bad_da, 1, cb_filter_strict_inf, &bound, cb_empty_deletor);
	da_nfilter_out(&bad_da, 0, cb_filter_strict_inf, &bound, cb_empty_deletor);
}
