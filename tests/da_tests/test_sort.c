#include <assert.h>

#include "da_tests.h"

NO_COV void test_cm_da_sort(void) {
	t_cm_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	da_swap(&da, 1, 3);
	da_swap(&da, 2, 1);
	da_swap(&da, 0, 3);
	assert(da_sort(&da, cb_cmp) == true);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	assert(da_sort(NULL, cb_cmp) == false);
	assert(da_sort(&da, NULL) == false);
	da_destroy(&da);
}
NO_COV void test_cm_da_nsort(void) {
	t_cm_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int[]){3, 2, 1, 4, 5}, 5, sizeof(int));
	assert(da_nsort(&da, 1, cb_cmp) == true);
	assert(da_nsort(&da, 2, cb_cmp) == true);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(da_nsort(&da, 3, cb_cmp) == true);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(da_nsort(NULL, 3, cb_cmp) == false);
	assert(da_nsort(&da, 3, NULL) == false);
	da_destroy(&da);
}
