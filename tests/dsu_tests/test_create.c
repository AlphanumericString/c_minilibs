#include <stdlib.h>

#include "dsu_tests.h"

// create
void test_dsu_new(void) {
	t_dsu *dsu;

	dsu = NULL;
	dsu = dsu_new(32);
	assert(dsu != NULL);
	assert(dsu->size == 32);
	assert(dsu->rp_single_alloc == true);
	assert(dsu->parent != NULL);
	assert(dsu->rank != NULL);
	assert(dsu->components == 32);
	for (size_t i = 0; i < 32; i++) {
		assert(dsu->parent[i] == i);
		assert(dsu->rank[i] == 0);
	}
	dsu_free(dsu);
}
void test_dsu_new_from_array(void) {
	t_dsu *dsu;
	int   *parents = malloc(sizeof(int) * 5);
	int   *ranks   = malloc(sizeof(int) * 5);

	dsu = dsu_new_from_array(5, parents, ranks);
	assert(dsu != NULL);
	assert(dsu->size == 5);
	assert(dsu->rp_single_alloc == false);
	assert(dsu->parent == (unsigned int *)parents);
	assert(dsu->rank == (unsigned int *)ranks);
	dsu_free(dsu);
}
void test_dsu_init(void) {
	t_dsu dsu;

	assert(dsu_init(&dsu, 10) == &dsu);
	assert(dsu.size == 10);
	assert(dsu.rp_single_alloc == true);
	assert(dsu.parent != NULL);
	assert(dsu.rank != NULL);
	dsu_destroy(&dsu);
}
void test_dsu_init_from_arrays(void) {
	t_dsu dsu;
	int  *parents = malloc(sizeof(int) * 5);
	int  *ranks   = malloc(sizeof(int) * 5);

	assert(dsu_init_from_arrays(&dsu, 5, parents, ranks) == &dsu);
	assert(dsu.size == 5);
	assert(dsu.rp_single_alloc == false);
	assert(dsu.parent == (unsigned int *)parents);
	assert(dsu.rank == (unsigned int *)ranks);
	dsu_destroy(&dsu);
}
