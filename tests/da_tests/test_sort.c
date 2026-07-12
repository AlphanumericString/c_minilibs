#include "da_tests.h"

// sort
void test_da_sort(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	da_swap(&da, 1, 3);
	da_swap(&da, 2, 1);
	da_swap(&da, 0, 3);
	da_sort(&da, cb_cmp);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	da_destroy(&da);
}
void test_da_nsort(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int[]){3, 2, 1, 4, 5}, 5, sizeof(int));
	da_nsort(&da, 2, cb_cmp);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 3);
	da_nsort(&da, 3, cb_cmp);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	da_destroy(&da);
}

