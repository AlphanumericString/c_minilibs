#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include "rdsu_tests.h"

NO_COV void test_cm_rdsu_grow(void) {
	t_cm_rdsu *rdsu = rdsu_new(5);
	t_cm_rdsu  intact_copy, no_parent, no_rank, no_size;

	// normal
	assert(rdsu->size == 5);
	assert(rdsu_grow(rdsu, 5) == true);
	assert(rdsu->size == 10);
	assert(rdsu->components == 10);
	for (size_t i = 0; i < 10; i++)
		assert(rdsu->parent[i] == i);

	// errs
	alloc_set_call_limit(0);
	assert(rdsu_grow(rdsu, 5) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	intact_copy      = *rdsu;
	no_parent        = *rdsu;
	no_rank          = *rdsu;
	no_size          = *rdsu;
	no_parent.parent = NULL;
	no_rank.rank     = NULL;
	no_size.size     = 0;
	assert(rdsu_grow(&no_parent, 5) == false);
	assert(rdsu_grow(&no_rank, 5) == false);
	assert(rdsu_grow(&no_size, 5) == false);
	assert(rdsu_grow(&intact_copy, 0) == false);
	rdsu_free(&rdsu);
	assert(rdsu_grow(rdsu, 0) == false);
	assert(rdsu_grow(NULL, 5) == false);

	// non-contiguous, single alloc for rank+parent
	rdsu = rdsu_new_from_arrays(5, malloc(sizeof(int) * 5),
	                            malloc(sizeof(int) * 5));
	assert(rdsu->rp_single_alloc == false);
	assert(rdsu_grow(rdsu, 5) == true);
	assert(rdsu->rp_single_alloc == true);
	assert(rdsu->size == 10);
	assert(rdsu->components == 10);
	for (size_t i = 0; i < 10; i++)
		assert(rdsu->parent[i] == i);
	rdsu_free(&rdsu);
}
