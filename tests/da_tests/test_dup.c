#include <assert.h>

#include "da_tests.h"

void test_da_dup(void) {
	const t_da da = {
	    .array = (void *)arr_3,
	    .cap_e = 3,
	    .nb_e  = 3,
	    .sz_e  = sizeof(int),
	};
	t_da dup;

	dup = da_dup(&da);
	ASSERT_DA_STATE(dup, da.sz_e, da.nb_e, da.nb_e);
	assert(*(int *)da_at(&dup, 0) == *(int *)da_at(&da, 0));
	da_destroy(&dup);
	dup = da_dup(NULL);
	assert(dup.array == NULL && dup.cap_e == 0 && dup.nb_e == 0 &&
	       dup.sz_e == 0);
}
void test_da_ndup(void) {
	const t_da da = {
	    .array = (void *)arr_3,
	    .cap_e = 3,
	    .nb_e  = 3,
	    .sz_e  = sizeof(int),
	};
	t_da dup;

	dup = da_ndup(&da, 2);
	ASSERT_DA_STATE(dup, da.sz_e, 2, 2);
	assert(*(int *)da_at(&dup, 0) == *(int *)da_at(&da, 0));
	assert(*(int *)da_at(&dup, 1) == *(int *)da_at(&da, 1));
	da_destroy(&dup);
	dup = da_ndup(NULL, 2);
	assert(dup.array == NULL && dup.cap_e == 0 && dup.nb_e == 0 &&
	       dup.sz_e == 0);
	dup = da_ndup(&da, 0);
	assert(dup.array == NULL && dup.cap_e == 0 && dup.nb_e == 0 &&
	       dup.sz_e == 0);
}
void test_da_dup_to(void) {
	const t_da da = {
	    .array = (void *)arr_3,
	    .cap_e = 3,
	    .nb_e  = 3,
	    .sz_e  = sizeof(int),
	};
	t_da dst;

	da_init(&dst, sizeof(int));
	da_dup_to(&dst, &da);
	ASSERT_DA_STATE(dst, da.sz_e, C_MINI_DA__NB_ELEM_DFLT, da.nb_e);
	assert(*(int *)da_at(&da, 0) == *(int *)da_at(&dst, 0));
	da_dup_to(NULL, &da);
	da_dup_to(&dst, NULL);
	da_destroy(&dst);
}
void test_da_ndup_to(void) {
	const t_da da = {
	    .array = (void *)arr_3,
	    .cap_e = 3,
	    .nb_e  = 3,
	    .sz_e  = sizeof(int),
	};
	t_da dst;

	da_init(&dst, sizeof(int));
	da_ndup_to(&dst, 2, &da);
	ASSERT_DA_STATE(dst, da.sz_e, C_MINI_DA__NB_ELEM_DFLT, 2);
	assert(*(int *)da_at(&dst, 0) == *(int *)da_at(&da, 0));
	assert(*(int *)da_at(&dst, 1) == *(int *)da_at(&da, 1));

	da_ndup_to(NULL, 2, &da);
	da_ndup_to(&dst, 2, NULL);
	da_ndup_to(&dst, 0, &da);

	da_destroy(&dst);
}
