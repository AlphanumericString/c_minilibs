#include <assert.h>
#include <stdlib.h>

#include "cdsu_tests.h"

void test_cdsu_new(void) {
	t_cdsu *cdsu;

	cdsu = NULL;
	cdsu = cdsu_new(32);
	assert(cdsu != NULL);
	assert(cdsu->size == 32);
	assert(cdsu->rp_single_alloc == true);
	assert(cdsu->parent != NULL);
	assert(cdsu->rank != NULL);
	assert(cdsu->components == 32);
	for (size_t i = 0; i < 32; i++) {
		assert(cdsu->parent[i] == i);
		assert(cdsu->rank[i] == 0);
	}
	cdsu_free(&cdsu);
	assert(cdsu_new(0) == NULL);
}
void test_cdsu_new_from_array(void) {
	t_cdsu *cdsu;
	int    *parents = malloc(sizeof(int) * 5);
	int    *ranks   = malloc(sizeof(int) * 5);

	cdsu = cdsu_new_from_array(5, parents, ranks);
	assert(cdsu != NULL);
	assert(cdsu->size == 5);
	assert(cdsu->rp_single_alloc == false);
	assert(cdsu->parent == (unsigned int *)parents);
	assert(cdsu->rank == (unsigned int *)ranks);
	cdsu_free(&cdsu);
	assert(cdsu_new_from_array(99, parents, parents) == NULL);
	assert(cdsu_new_from_array(99, ranks, ranks) == NULL);
	assert(cdsu_new_from_array(0, parents, ranks) == NULL);
	assert(cdsu_new_from_array(99, NULL, ranks) == NULL);
	assert(cdsu_new_from_array(99, parents, NULL) == NULL);
}
void test_cdsu_init(void) {
	t_cdsu cdsu;

	assert(cdsu_init(&cdsu, 10) == &cdsu);
	assert(cdsu.size == 10);
	assert(cdsu.rp_single_alloc == true);
	assert(cdsu.parent != NULL);
	assert(cdsu.rank != NULL);
	cdsu_destroy(&cdsu);
	assert(cdsu_init(NULL, 10) == NULL);
	assert(cdsu_init(&cdsu, 0) == NULL);
}
void test_cdsu_init_from_arrays(void) {
	t_cdsu cdsu;
	int   *parents = malloc(sizeof(int) * 5);
	int   *ranks   = malloc(sizeof(int) * 5);

	assert(cdsu_init_from_arrays(&cdsu, 5, parents, ranks) == &cdsu);
	assert(cdsu.size == 5);
	assert(cdsu.rp_single_alloc == false);
	assert(cdsu.parent == (unsigned int *)parents);
	assert(cdsu.rank == (unsigned int *)ranks);
	cdsu_destroy(&cdsu);
	assert(cdsu_init_from_arrays(&cdsu, 5, parents, parents) == NULL);
	assert(cdsu_init_from_arrays(&cdsu, 5, ranks, ranks) == NULL);
	assert(cdsu_init_from_arrays(NULL, 5, parents, ranks) == NULL);
	assert(cdsu_init_from_arrays(&cdsu, 0, parents, ranks) == NULL);
	assert(cdsu_init_from_arrays(&cdsu, 5, NULL, ranks) == NULL);
	assert(cdsu_init_from_arrays(&cdsu, 5, parents, NULL) == NULL);
}
