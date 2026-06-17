#include "da_tests.h"

// add
void test_da_add(void) {
	t_da da;
	int  a;

	da_init(&da, sizeof(int));

	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	a = 5;
	da_add(&da, &a);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 1);
	assert(*(int *)(da.array) == a);

	da_destroy(&da);
}
void test_da_cat_array(void) {
	t_da da;
	int  arr[] = {1, 2, 3};

	da_init(&da, sizeof(int));
	da_cat_array(&da, arr, 3, sizeof(int));
	da_destroy(&da);
}

