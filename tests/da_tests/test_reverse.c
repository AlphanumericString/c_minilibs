#include "da_tests.h"

// reverse
void test_da_reverse(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	da_reverse(&da);
	assert(*(int *)da_at(&da, 0) == 5);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 2);
	assert(*(int *)da_at(&da, 4) == 1);

	da_destroy(&da);
}
void test_da_nreverse(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	da_nreverse(&da, 2);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 1);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);

	da_destroy(&da);
}
void test_da_b_nreverse(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	da_b_nreverse(&da, 2);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 5);
	assert(*(int *)da_at(&da, 4) == 4);

	da_destroy(&da);
}
