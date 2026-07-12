#include "da_tests.h"

// swap
void test_da_swap(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));
	da_swap(&da, 0, 1);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 1);
	da_destroy(&da);
}
void test_da_nswap(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	da_nswap(&da, 1, 0, 1); // 2 1 3 4 5
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 1);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	da_nswap(&da, 2, 0, 2); // 3 4 2 1 5
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 2);
	assert(*(int *)da_at(&da, 3) == 1);
	da_destroy(&da);
}
