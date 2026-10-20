#include <assert.h>

#include "da_tests.h"

NO_COV void test_da_shrink(void) {
	t_da da;

	da_init(&da, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	alloc_set_call_limit(0);
	assert(da_shrink(&da) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	assert(da_shrink(&da) == true);
	assert(da_shrink(&da) == true);
	assert(da_shrink(&da) == true);
	assert(da_shrink(&da) == true);
	ASSERT_DA_STATE(da, sizeof(int), 0, 0);
	da_destroy(&da);
	assert(da_shrink(NULL) == false);
}
NO_COV void test_da_shrink_min(void) {
	t_da da;

	da_init(&da, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	alloc_set_call_limit(0);
	assert(da_shrink_min(&da, 1) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	assert(da_shrink_min(&da, C_MINI_DA__NB_ELEM_DFLT + 2) == true);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_grow(&da);
	da_grow(&da);
	assert(da_shrink_min(&da, 5) == true);
	ASSERT_DA_STATE(da, sizeof(int), 5, 0);
	da_destroy(&da);
	assert(da_shrink_min(NULL, 5) == false);
}
