#include <assert.h>
#include <stdlib.h>

#include "da_tests.h"

NO_COV void test_da_dup(void) {
	const t_da da = {
	    .array = (void *)arr_3,
	    .cap_e = 3,
	    .nb_e  = 3,
	    .sz_e  = sizeof(int),
	};
	t_da dup;

	alloc_set_call_limit(0);
	dup = da_dup(&da);
	assert(dup.array == NULL && dup.cap_e == 0);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	dup = da_dup(&da);
	ASSERT_DA_STATE(dup, da.sz_e, da.nb_e, da.nb_e);
	assert(*(int *)da_at(&dup, 0) == *(int *)da_at(&da, 0));
	da_destroy(&dup);
	dup = da_dup(NULL);
	assert(dup.array == NULL && dup.cap_e == 0 && dup.nb_e == 0 &&
	       dup.sz_e == 0);
}
NO_COV void test_da_ndup(void) {
	const t_da da = {
	    .array = (void *)arr_3,
	    .cap_e = 3,
	    .nb_e  = 3,
	    .sz_e  = sizeof(int),
	};
	t_da dup;

	alloc_set_call_limit(0);
	dup = da_ndup(&da, 2);
	assert(dup.array == NULL && dup.cap_e == 0);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

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
NO_COV void test_da_dup_to(void) {
	const t_da da = {
	    .array = (void *)arr_3,
	    .cap_e = 3,
	    .nb_e  = 3,
	    .sz_e  = sizeof(int),
	};
	t_da dst;
	t_da small2 = {
	    .array = calloc(sizeof(int), 1),
	    .cap_e = 1,
	    .nb_e  = 0,
	    .sz_e  = sizeof(int),
	};
	void *prev;

	da_init(&dst, sizeof(int));
	da_dup_to(&dst, &da);
	ASSERT_DA_STATE(dst, da.sz_e, C_MINI_DA__NB_ELEM_DFLT, da.nb_e);
	assert(*(int *)da_at(&da, 0) == *(int *)da_at(&dst, 0));

	prev = small2.array;
	alloc_set_call_limit(0);
	assert(da_dup_to(&small2, &da) == NULL);
	assert(small2.array != NULL), assert(small2.sz_e == sizeof(int));
	assert(small2.cap_e >= 1), assert(small2.nb_e == 0);
	assert(prev == small2.array);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	da_dup_to(NULL, &da);
	da_dup_to(&dst, NULL);
	da_destroy(&dst);
	da_destroy(&small2);
}
NO_COV void test_da_ndup_to(void) {
	const t_da da = {
	    .array = (void *)arr_3,
	    .cap_e = 3,
	    .nb_e  = 3,
	    .sz_e  = sizeof(int),
	};
	t_da dst;
	t_da small1 = {
	    .array = calloc(sizeof(int), 1),
	    .cap_e = 1,
	    .nb_e  = 0,
	    .sz_e  = sizeof(int),
	};
	t_da small2 = {
	    .array = calloc(sizeof(int), 1),
	    .cap_e = 1,
	    .nb_e  = 0,
	    .sz_e  = sizeof(int),
	};
	void *prev;

	da_init(&dst, sizeof(int));

	prev = dst.array;
	assert(da_ndup_to(&dst, 2, &da) == &dst);
	ASSERT_DA_STATE(dst, da.sz_e, C_MINI_DA__NB_ELEM_DFLT, 2);
	assert(prev == dst.array); // no need to grow
	assert(*(int *)da_at(&dst, 0) == *(int *)da_at(&da, 0));
	assert(*(int *)da_at(&dst, 1) == *(int *)da_at(&da, 1));

	prev = small1.array;
	assert(da_ndup_to(&small1, 2, &da) == &small1);
	assert(small1.array != NULL), assert(small1.sz_e == sizeof(int));
	assert(small1.cap_e >= 1), assert(small1.nb_e == 2);
	assert(prev != small1.array);

	prev = small2.array;
	alloc_set_call_limit(0);
	assert(da_ndup_to(&small2, 2, &da) == NULL);
	assert(small2.array != NULL), assert(small2.sz_e == sizeof(int));
	assert(small2.cap_e >= 1), assert(small2.nb_e == 0);
	assert(prev == small2.array);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	assert(da_ndup_to(NULL, 2, &da) == NULL);
	assert(da_ndup_to(&dst, 2, NULL) == &dst);
	assert(da_ndup_to(&dst, 0, &da) == &dst);

	da_destroy(&small2);
	da_destroy(&small1);
	da_destroy(&dst);
}
