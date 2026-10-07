#include <assert.h>
#include <stdlib.h>

#include "rdsu_tests.h"

void test_rdsu_grow(void) {
	t_rdsu *rdsu = rdsu_new(5);
	t_rdsu  intact_copy;

	// normal
	assert(rdsu->size == 5);
	assert(rdsu_grow(rdsu, 5) == true);
	assert(rdsu->size == 10);
	assert(rdsu->components == 10);
	for (size_t i = 0; i < 10; i++)
		assert(rdsu->parent[i] == i);

	// errs
	intact_copy  = *rdsu;
	rdsu->parent = NULL;
	assert(rdsu_grow(rdsu, 5) == false);
	*rdsu      = intact_copy;
	rdsu->rank = NULL;
	assert(rdsu_grow(rdsu, 5) == false);
	*rdsu      = intact_copy;
	rdsu->size = 0;
	assert(rdsu_grow(rdsu, 5) == false);
	*rdsu = intact_copy;
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
