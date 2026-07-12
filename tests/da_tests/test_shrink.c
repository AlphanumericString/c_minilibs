#include "da_tests.h"

// shrink
void test_da_shrink(void) {
	t_da da;

	da_init(&da, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_shrink(&da);
	ASSERT_DA_STATE(da, sizeof(int), 0, 0);
	da_destroy(&da);
}
void test_da_shrink_min(void) {
	t_da da;

	da_init(&da, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_shrink_min(&da, 5);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_grow(&da);
	da_grow(&da);
	da_shrink_min(&da, 5);
	ASSERT_DA_STATE(da, sizeof(int), 5, 0);
	da_destroy(&da);
}
