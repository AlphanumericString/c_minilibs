#include <assert.h>

#include "da_tests.h"

NO_COV static void test_cm_da_eq(const t_cm_da *da, const int *vals,
                                 size_t sz) {
	assert(da->nb_e <= sz);
	for (size_t i = 0; i < sz; i++)
		assert(vals[i] == *(int *)da_at(da, i));
}

NO_COV static void check_00_gap(t_cm_da *da) { // 0
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 0, 0) == true);
	assert(da->nb_e == 5);
	test_cm_da_eq(da, arr_5, 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 1, 1) == true);
	assert(da->nb_e == 5);
	test_cm_da_eq(da, arr_5, 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 2, 2) == true);
	assert(da->nb_e == 5);
	test_cm_da_eq(da, arr_5, 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 3, 3) == true);
	assert(da->nb_e == 5);
	test_cm_da_eq(da, arr_5, 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 4, 4) == true);
	assert(da->nb_e == 5);
	test_cm_da_eq(da, arr_5, 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 5, 5) == false);
	assert(da->nb_e == 5);
	test_cm_da_eq(da, arr_5, 5);
	da_clr(da);
}
NO_COV static void check_01_gap(t_cm_da *da) { // -1
	int arrs[][4] = {
	    {2, 3, 4, 5},
        {1, 3, 4, 5},
        {1, 2, 4, 5},
        {1, 2, 3, 5},
        {1, 2, 3, 4},
	};

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 0, 1) == true);
	assert(da->nb_e == 4);
	da_sort(da, cb_cmp);
	test_cm_da_eq(da, arrs[0], 4);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 1, 2) == true);
	assert(da->nb_e == 4);
	da_sort(da, cb_cmp);
	test_cm_da_eq(da, arrs[1], 4);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 2, 3) == true);
	assert(da->nb_e == 4);
	da_sort(da, cb_cmp);
	test_cm_da_eq(da, arrs[2], 4);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 3, 4) == true);
	assert(da->nb_e == 4);
	da_sort(da, cb_cmp);
	test_cm_da_eq(da, arrs[3], 4);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 4, 5) == true);
	assert(da->nb_e == 4);
	da_sort(da, cb_cmp);
	test_cm_da_eq(da, arrs[4], 4);
	da_clr(da);
}
NO_COV static void check_02_gap(t_cm_da *da) { // -2
	int arrs[][3] = {
	    {3, 4, 5},
	    {1, 4, 5},
	    {1, 2, 5},
	    {1, 2, 3},
	};

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 0, 2) == true);
	assert(da->nb_e == 3);
	da_sort(da, cb_cmp);
	test_cm_da_eq(da, arrs[0], 3);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 1, 3) == true);
	assert(da->nb_e == 3);
	da_sort(da, cb_cmp);
	test_cm_da_eq(da, arrs[1], 3);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 2, 4) == true);
	assert(da->nb_e == 3);
	da_sort(da, cb_cmp);
	test_cm_da_eq(da, arrs[2], 3);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 3, 5) == true);
	assert(da->nb_e == 3);
	da_sort(da, cb_cmp);
	test_cm_da_eq(da, arrs[3], 3);
	da_clr(da);
}
NO_COV static void check_03_gap(t_cm_da *da) { // -3
	int arrs[][2] = {
	    {4, 5},
	    {1, 5},
	    {1, 2},
	};
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 0, 3) == true);
	assert(da->nb_e == 2);
	da_sort(da, cb_cmp);
	test_cm_da_eq(da, arrs[0], 2);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 1, 4) == true);
	assert(da->nb_e == 2);
	da_sort(da, cb_cmp);
	test_cm_da_eq(da, arrs[1], 2);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 2, 5) == true);
	assert(da->nb_e == 2);
	da_sort(da, cb_cmp);
	test_cm_da_eq(da, arrs[2], 2);
	da_clr(da);
}
NO_COV static void check_04_gap(t_cm_da *da) { // -4
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 0, 4) == true);
	assert(da->nb_e == 1);
	assert(*(int *)da_at(da, 0) == 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 1, 5) == true);
	assert(da->nb_e == 1);
	assert(*(int *)da_at(da, 0) == 1);
	da_clr(da);
}
NO_COV static void check_05_gap(t_cm_da *da) { // -5
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 0, 5) == true);
	assert(da->nb_e == 0);
	da_clr(da);
}
NO_COV static void check_oob(t_cm_da *da) {
	int arrs[][5] = {
	    {0},
        {1, 0},
        {1, 2, 0},
        {1, 2, 3, 0},
        {1, 2, 3, 4, 0},
        {1, 2, 3, 4, 5},
	};
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 0, -1) == true);
	assert(da->nb_e == 0);
	test_cm_da_eq(da, arrs[0], 0);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 1, -1) == true);
	assert(da->nb_e == 1);
	test_cm_da_eq(da, arrs[1], 1);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 2, -1) == true);
	assert(da->nb_e == 2);
	test_cm_da_eq(da, arrs[2], 2);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 3, -1) == true);
	assert(da->nb_e == 3);
	test_cm_da_eq(da, arrs[3], 3);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 4, -1) == true);
	assert(da->nb_e == 4);
	test_cm_da_eq(da, arrs[4], 4);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(da, 5, -1) == false);
	assert(da->nb_e == 5);
	test_cm_da_eq(da, arrs[5], 5);
	da_clr(da);
}
NO_COV static void check_errors(t_cm_da *da) {
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms(NULL, 0, -1) == false); // no da provided
	assert(da_rms(da, 3, 1) == false);    // start > end
	assert(da_rms(da, 99, 100) == false); // start oob
	da_clr(da);
}

NO_COV static void check_00_order_gap(t_cm_da *da) {
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 0, 0) == true);
	assert(da->nb_e == 5);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 2);
	assert(*(int *)da_at(da, 2) == 3);
	assert(*(int *)da_at(da, 3) == 4);
	assert(*(int *)da_at(da, 4) == 5);
	da_clr(da);
}
NO_COV static void check_01_order_gap(t_cm_da *da) {
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 0, 1) == true);
	assert(da->nb_e == 4);
	assert(*(int *)da_at(da, 0) == 2);
	assert(*(int *)da_at(da, 1) == 3);
	assert(*(int *)da_at(da, 2) == 4);
	assert(*(int *)da_at(da, 3) == 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 1, 2) == true);
	assert(da->nb_e == 4);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 3);
	assert(*(int *)da_at(da, 2) == 4);
	assert(*(int *)da_at(da, 3) == 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 2, 3) == true);
	assert(da->nb_e == 4);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 2);
	assert(*(int *)da_at(da, 2) == 4);
	assert(*(int *)da_at(da, 3) == 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 3, 4) == true);
	assert(da->nb_e == 4);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 2);
	assert(*(int *)da_at(da, 2) == 3);
	assert(*(int *)da_at(da, 3) == 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 4, 5) == true);
	assert(da->nb_e == 4);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 2);
	assert(*(int *)da_at(da, 2) == 3);
	assert(*(int *)da_at(da, 3) == 4);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 5, 6) == false); // oob start
	assert(da->nb_e == 5);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 2);
	assert(*(int *)da_at(da, 2) == 3);
	assert(*(int *)da_at(da, 3) == 4);
	assert(*(int *)da_at(da, 4) == 5);
	da_clr(da);
}
NO_COV static void check_02_order_gap(t_cm_da *da) {
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 0, 2) == true);
	assert(da->nb_e == 3);
	assert(*(int *)da_at(da, 0) == 3);
	assert(*(int *)da_at(da, 1) == 4);
	assert(*(int *)da_at(da, 2) == 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 1, 3) == true);
	assert(da->nb_e == 3);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 4);
	assert(*(int *)da_at(da, 2) == 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 3, 5) == true);
	assert(da->nb_e == 3);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 2);
	assert(*(int *)da_at(da, 2) == 3);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 2, 4) == true);
	assert(da->nb_e == 3);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 2);
	assert(*(int *)da_at(da, 2) == 5);
	da_clr(da);
}
NO_COV static void check_03_order_gap(t_cm_da *da) {
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 0, 3) == true);
	assert(da->nb_e == 2);
	assert(*(int *)da_at(da, 0) == 4);
	assert(*(int *)da_at(da, 1) == 5);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 2, 5) == true);
	assert(da->nb_e == 2);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 2);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 1, 4) == true);
	assert(da->nb_e == 2);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 5);
	da_clr(da);
}
NO_COV static void check_04_order_gap(t_cm_da *da) {
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 1, 5) == true);
	assert(da->nb_e == 1);
	assert(*(int *)da_at(da, 0) == 1);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 0, 4) == true);
	assert(da->nb_e == 1);
	assert(*(int *)da_at(da, 0) == 5);
	da_clr(da);
}
NO_COV static void check_05_order_gap(t_cm_da *da) {
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 0, 5) == true);
	assert(da->nb_e == 0);
	da_clr(da);
}
NO_COV static void check_oob_order(t_cm_da *da) {
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 1, -1) == true); // leaves ony 1 elem
	assert(da->nb_e == 1);
	assert(*(int *)da_at(da, 0) == 1);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 2, 100) == true);
	assert(da->nb_e == 2);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 2);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 3, 9999) == true);
	assert(da->nb_e == 3);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 2);
	assert(*(int *)da_at(da, 2) == 3);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 4, -1) == true);
	assert(da->nb_e == 4);
	assert(*(int *)da_at(da, 0) == 1);
	assert(*(int *)da_at(da, 1) == 2);
	assert(*(int *)da_at(da, 2) == 3);
	assert(*(int *)da_at(da, 3) == 4);
	da_clr(da);

	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(da, 5, -1) == false);   // start==nb_e
	assert(da_rms_order(da, 99, 1) == false);   // start>nb_e && end<start
	assert(da_rms_order(da, 99, 100) == false); // start>nb_e
	da_clr(da);
}
NO_COV static void check_errors_order(t_cm_da *da) {
	da_cat_array(da, (int *)arr_5, 5, sizeof(int));
	assert(da_rms_order(NULL, 0, 1) == false); // no da provided
	assert(da_rms_order(da, 5, 2) == false);   // end<start
	da_clr(da);
}

//
//
//
//

NO_COV void test_cm_da_rm_order(void) {
	t_cm_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	assert(da_rm_order(&da, 0) == true);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	assert(*(int *)da_at(&da, 3) == 5);
	assert(da.nb_e == 4);
	assert(da_rm_order(&da, 0) == true);
	assert(da.nb_e == 3);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 5);
	assert((int *)da_at(&da, 3) == NULL);
	assert(da_rm_order(&da, 1) == true);
	assert(da.nb_e == 2);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 5);
	assert((int *)da_at(&da, 2) == NULL);
	assert(da_rm_order(&da, 1) == true);
	assert(da.nb_e == 1);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(da_rm_order(&da, -1) == false);
	assert(da_rm_order(NULL, 0) == false);
	da_destroy(&da);
}
NO_COV void test_cm_da_rm(void) {
	t_cm_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	assert(da_rm(&da, 0) == true);
	assert(*(int *)da_at(&da, 0) != 1);
	assert(da.nb_e == 4);
	assert(*(int *)da_at(&da, 0) == 5);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(da_rm(&da, 1) == true);
	assert(da.nb_e == 3);
	assert(*(int *)da_at(&da, 0) == 5);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(da_rm(&da, 3) == true);
	assert(da.nb_e == 2);
	assert(*(int *)da_at(&da, 0) == 5);
	assert(*(int *)da_at(&da, 1) == 4);

	assert(da_rm(&da, 99) == false);
	assert(da_rm(NULL, 1) == false);
	da_destroy(&da);
}

NO_COV void test_cm_da_rms_order(void) {
	t_cm_da da;

	da_init(&da, sizeof(int));
	assert(da.array != NULL);

	check_00_order_gap(&da);
	check_01_order_gap(&da);
	check_02_order_gap(&da);
	check_03_order_gap(&da);
	check_04_order_gap(&da);
	check_05_order_gap(&da);
	check_oob_order(&da);
	check_errors_order(&da);

	da_destroy(&da);
}
NO_COV void test_cm_da_rms(void) {
	t_cm_da da;

	da_init(&da, sizeof(int));
	assert(da.array != NULL);

	check_00_gap(&da);
	check_01_gap(&da);
	check_02_gap(&da);
	check_03_gap(&da);
	check_04_gap(&da);
	check_05_gap(&da);
	check_oob(&da);
	check_errors(&da);

	da_destroy(&da);
}
