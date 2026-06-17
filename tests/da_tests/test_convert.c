#include "da_tests.h"

// convert
void test_da_to_array(void) {
	t_da da;
	int *arr;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	arr = (int *)da_to_array(&da);
	assert(arr != NULL);
	assert(arr[0] == 1);
	assert(arr[1] == 2);
	assert(arr[2] == 3);
	assert(arr[3] == 4);
	assert(arr[4] == 5);

	free(arr);
	da_destroy(&da);
}
void test_da_to_loc_array(void) {
	t_da da;
	int  a      = 1;
	int  arr[5] = {42, 42, 42, 42, 42};

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	da_to_loc_array(&da, arr);
	assert(arr != NULL);
	assert(arr[0] == 1);
	assert(arr[1] == 42);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	da_to_loc_array(&da, arr);
	assert(arr != NULL);
	assert(arr[0] == 1);
	assert(arr[1] == 2);
	assert(arr[2] == 3);
	assert(arr[3] == 4);
	assert(arr[4] == 5);

	da_destroy(&da);
}
