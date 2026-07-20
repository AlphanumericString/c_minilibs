#include <assert.h>

#include "da_tests.h"

void test_da_dup(void) {
	t_da da, dup;

	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	dup = da_dup(&da);
	ASSERT_DA_STATE(dup, da.sz_e, da.nb_e, da.nb_e);
	assert(*(int *)da_at(&dup, 0) == *(int *)da_at(&da, 0));
	da_destroy(&da);
	da_destroy(&dup);
}
void test_da_ndup(void) {
	t_da da, dup;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));
	dup = da_ndup(&da, 2);
	ASSERT_DA_STATE(dup, da.sz_e, 2, 2);
	assert(*(int *)da_at(&dup, 0) == *(int *)da_at(&da, 0));
	assert(*(int *)da_at(&dup, 1) == *(int *)da_at(&da, 1));
	da_destroy(&da);
	da_destroy(&dup);
}
void test_da_dup_to(void) {
	t_da da, dst;
	int  a;

	da_init(&da, sizeof(int));
	da_init(&dst, sizeof(int));
	a = 1;
	da_add(&da, &a);
	da_dup_to(&dst, &da);
	ASSERT_DA_STATE(dst, da.sz_e, C_MINI_DA__NB_ELEM_DFLT, da.nb_e);
	assert(*(int *)da_at(&da, 0) == *(int *)da_at(&dst, 0));
	da_destroy(&da);
	da_destroy(&dst);
}
void test_da_ndup_to(void) {
	t_da da, dst;

	da_init(&da, sizeof(int));
	da_init(&dst, sizeof(int));
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));
	da_ndup_to(&dst, 2, &da);
	ASSERT_DA_STATE(dst, da.sz_e, C_MINI_DA__NB_ELEM_DFLT, 2);
	assert(*(int *)da_at(&dst, 0) == *(int *)da_at(&da, 0));
	assert(*(int *)da_at(&dst, 1) == *(int *)da_at(&da, 1));
	da_destroy(&da);
	da_destroy(&dst);
}
