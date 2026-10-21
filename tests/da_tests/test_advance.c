#include <assert.h>

#include "da_tests.h"

NO_COV void test_da_advance(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 5)
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	da_advance(&da, 1, cb_empty_deletor, NULL);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 4)
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	da_advance(&da, 2, cb_empty_deletor, NULL);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 2)
	assert(*(int *)da_at(&da, 0) == 4);
	assert(*(int *)da_at(&da, 1) == 5);
	da_advance(&da, -1, cb_empty_deletor, NULL);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 0)
	da_destroy(&da);

	da_advance(NULL, 2, cb_empty_deletor, NULL);
}
