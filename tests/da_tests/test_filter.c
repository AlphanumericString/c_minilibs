#include <assert.h>

#include "da_tests.h"

void test_da_filter(void) {
	t_da da;
	int  a     = 1;
	int  bound = 3;

	da_init(&da, sizeof(int));
	da_add(&da, &a);
	da_filter(&da, cb_filter_strict_inf, &bound, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 1);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 5);
	da_filter(&da, cb_filter_strict_inf, &bound, cb_empty_deletor);
	da_sort(&da, cb_cmp);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 2);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	da_destroy(&da);
}
void test_da_nfilter(void) {
	t_da da;
	int  a     = 1;
	int  bound = 3;

	da_init(&da, sizeof(int));
	da_add(&da, &a);
	da_nfilter(&da, 1, cb_true, NULL, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 1);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 5);
	da_nfilter(&da, 4, cb_filter_strict_inf, &bound, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 3);
	assert(*(int *)da_at(&da, 2) == 5);
	da_destroy(&da);
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
	da_filter_out(&da, cb_filter_strict_inf, &bound, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 3);
	da_sort(&da, cb_cmp);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 5);
	da_destroy(&da);
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
	da_destroy(&da);
}
