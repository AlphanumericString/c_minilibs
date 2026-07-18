#include "da_tests.h"

// create
void test_da_init(void) {
	t_da da;

	da_init(&da, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_destroy(&da);
}
void test_da_ninit(void) {
	t_da da;

	da_ninit(&da, 10, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), 10, 0);
	da_destroy(&da);
}
void test_da_from_array(void) {
	t_da da;
	int *arr = calloc(5, sizeof(int));

	da_from_array(&da, arr, 5, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), 5, 0);
	assert(da.array == arr);
	da_destroy(&da);
}
void test_da_new(void) {

	t_da *da = da_new(sizeof(int));

	ASSERT_DA_STATE(*da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_free(da);
}
