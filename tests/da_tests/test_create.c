#include <assert.h>
#include <stdlib.h>

#include "da_tests.h"

NO_COV void test_da_init(void) {
	t_da  da;
	t_da *ret;

	ret = da_init(&da, sizeof(int));
	assert(ret != NULL && ret == &da);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0)
	da_destroy(&da);
	assert(da_init(NULL, sizeof(int)) == NULL);
	assert(da_init(&da, 0) == NULL);
}
NO_COV void test_da_ninit(void) {
	t_da  da;
	t_da *ret;

	alloc_reset_counter();
	ret = da_ninit(&da, 10, sizeof(int));
	assert(nb_alloc == 1);
	assert(ret == &da);
	ASSERT_DA_STATE(da, sizeof(int), 10, 0)
	assert(da.nb_e == 0);
	assert(da.cap_e == 10);
	da_destroy(&da);

	assert(da_ninit(NULL, 10, sizeof(int)) == NULL);
	assert(da_ninit(&da, 10, 0) == NULL);
	alloc_reset_counter();
	alloc_set_call_limit(0);
	assert(da_ninit(&da, 10, sizeof(int)) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
}
NO_COV void test_da_from_array(void) {
	t_da  da;
	int  *arr = (int *)calloc(5, sizeof(int));
	t_da *ret;

	ret = da_from_array(&da, arr, 5, sizeof(int));
	assert(ret != NULL && ret == &da);
	ASSERT_DA_STATE(da, sizeof(int), 5, 0)
	assert(da.array == arr);
	da_destroy(&da);
	assert(da_from_array(NULL, arr, 5, sizeof(int)) == NULL);
	assert(da_from_array(&da, NULL, 5, sizeof(int)) == NULL);
	assert(da_from_array(&da, arr, 5, 0) == NULL);
}
NO_COV void test_da_new(void) {
	t_da *da;

	alloc_reset_counter();
	assert(nb_alloc == 0);
	da = da_new(sizeof(int));
	assert(nb_alloc == 2);
	ASSERT_DA_STATE(*da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0)
	da_free(da);
	assert(da_new(0) == NULL);
	alloc_reset_counter();
	alloc_set_call_limit(1);
	assert(nb_alloc == 0 && alloc_limit == 1);
	assert(da_new(sizeof(int)) == NULL);
	assert(nb_alloc == 1 && alloc_limit == 1);
	assert(da_new(sizeof(int)) == NULL);
	assert(nb_alloc == 1 && alloc_limit == 1);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
}
