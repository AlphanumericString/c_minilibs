#include <assert.h>
#include <stdlib.h>

#include "da_tests.h"

void test_da_init(void) {
	t_da  da;
	t_da *ret;

	ret = da_init(&da, sizeof(int));
	assert(ret != NULL && ret == &da);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_destroy(&da);
	assert(da_init(NULL, sizeof(int)) == NULL);
	assert(da_init(&da, 0) == NULL);
}
void test_da_ninit(void) {
	t_da  da;
	t_da *ret;

	ret = da_ninit(&da, 10, sizeof(int));
	assert(ret != NULL);
	assert(ret == &da);
	ASSERT_DA_STATE(da, sizeof(int), 10, 0);
	assert(da.nb_e == 0);
	assert(da.cap_e == 10);
	da_destroy(&da);
	assert(da_ninit(NULL, 10, sizeof(int)) == NULL);
	assert(da_ninit(&da, 10, 0) == NULL);
}
void test_da_from_array(void) {
	t_da  da;
	int  *arr = calloc(5, sizeof(int));
	t_da *ret;

	ret = da_from_array(&da, arr, 5, sizeof(int));
	assert(ret != NULL && ret == &da);
	ASSERT_DA_STATE(da, sizeof(int), 5, 0);
	assert(da.array == arr);
	da_destroy(&da);
	assert(da_from_array(NULL, arr, 5, sizeof(int)) == NULL);
	assert(da_from_array(&da, NULL, 5, sizeof(int)) == NULL);
	assert(da_from_array(&da, arr, 5, 0) == NULL);
}
void test_da_new(void) {
	t_da *da = da_new(sizeof(int));

	ASSERT_DA_STATE(*da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_free(da);
	assert(da_new(0) == NULL);
}
