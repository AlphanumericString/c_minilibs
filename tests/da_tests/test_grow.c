#include <assert.h>

#include "da_tests.h"

NO_COV void test_da_grow(void) {
	t_da         da;
	const size_t dflt_size   = C_MINI__DA_NB_ELEM_DFLT;
	const size_t first_size  = dflt_size * C_MINI__DA_GROWTH;
	const size_t second_size = first_size * C_MINI__DA_GROWTH;
	t_da         da_no_size  = {
	             .array = (int *)arr_5,
	             .cap_e = 5,
	             .nb_e  = 5,
	             .sz_e  = 0,
    };
	t_da da_nb_sup_capacity = {
	    .array = (int *)arr_5,
	    .cap_e = 0,
	    .nb_e  = 5,
	    .sz_e  = sizeof(int),
	};

	da_init(&da, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), dflt_size, 0)
	assert(da_grow(&da) == true);
	ASSERT_DA_STATE(da, sizeof(int), first_size, 0)
	assert(da_grow(&da) == true);
	ASSERT_DA_STATE(da, sizeof(int), second_size, 0)
	alloc_set_call_limit(0);
	assert(da_grow(&da) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	ASSERT_DA_STATE(da, sizeof(int), second_size, 0)
	da_destroy(&da);
	assert(da.array == NULL && da.cap_e == 0 && da.nb_e == 0 && da.sz_e == 0);
	da.sz_e = sizeof(int);
	assert(da_grow(&da) == true);
	assert(da.cap_e == C_MINI__DA_NB_ELEM_DFLT);
	da_destroy(&da);

	assert(da_grow(&da_no_size) == false);
	assert(da_grow(&da_nb_sup_capacity) == false);
	assert(da_grow(NULL) == false);
}
NO_COV void test_da_grow_for_size(void) {
	t_da  da;
	void *prev;
	t_da  da_no_size = {
	     .array = (int *)arr_5,
	     .cap_e = 5,
	     .nb_e  = 5,
	     .sz_e  = 0,
    };
	t_da da_nb_sup_capacity = {
	    .array = (int *)arr_5,
	    .cap_e = 0,
	    .nb_e  = 5,
	    .sz_e  = sizeof(int),
	};

	da_init(&da, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), C_MINI__DA_NB_ELEM_DFLT, 0)
	assert(da_grow_for_size(&da, 20) == true);
	assert(da.cap_e >= 20), assert(da.sz_e == sizeof(int));
	assert(da.nb_e == 0), assert(da.array != NULL);

	assert(da_grow_for_size(&da, -1) == false);
	da_destroy(&da);
	assert(da.cap_e == 0), assert(da.sz_e == 0);
	assert(da.nb_e == 0), assert(da.array == NULL);

	da.sz_e = sizeof(int);
	assert(da_grow_for_size(&da, 2) == true);
	assert(da.cap_e >= 2), assert(da.sz_e == sizeof(int));
	assert(da.nb_e == 0), assert(da.array != NULL);

	prev = da.array;
	alloc_set_call_limit(0);
	assert(da_grow_for_size(&da, 32) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(da.cap_e >= 2), assert(da.sz_e == sizeof(int));
	assert(da.nb_e == 0), assert(da.array == prev);

	da_destroy(&da);

	assert(da_grow_for_size(NULL, 20) == false);
	assert(da_grow_for_size(&da_nb_sup_capacity, 20) == false);
	assert(da_grow_for_size(&da_no_size, 20) == false);
}
