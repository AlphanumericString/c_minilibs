#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "rdsu_tests.h"

void test_rdsu_new(void) {
	t_rdsu *rd;

	rd = rdsu_new(32);
	assert(rd != NULL);
	assert(rd->rank);
	assert(rd->parent);
	assert(rd->change_log == NULL);
	assert(rd->components == 32);
	rdsu_free(rd);
}
void test_rdsu_new_from_array(void) {
	t_rdsu   *rdsu;
	uint16_t *parents = malloc(sizeof(uint16_t) * 5);
	uint16_t *ranks   = malloc(sizeof(uint16_t) * 5);

	rdsu = rdsu_new_from_array(5, parents, ranks);
	assert(rdsu != NULL);
	assert(rdsu->size == 5);
	assert(rdsu->rp_single_alloc == false);
	assert(rdsu->parent == (uint16_t *)parents);
	assert(rdsu->rank == (uint16_t *)ranks);
	assert(rdsu->change_log == NULL);
	rdsu_free(rdsu);
}
void test_rdsu_init(void) {
	t_rdsu rdsu;

	assert(rdsu_init(&rdsu, 10) == &rdsu);
	assert(rdsu.size == 10);
	assert(rdsu.rp_single_alloc == true);
	assert(rdsu.parent != NULL);
	assert(rdsu.rank != NULL);
	assert(rdsu.change_log == NULL);
	rdsu_destroy(&rdsu);
}
void test_rdsu_init_from_arrays(void) {
	t_rdsu    rdsu;
	uint16_t *parents = malloc(sizeof(uint16_t) * 5);
	uint16_t *ranks   = malloc(sizeof(uint16_t) * 5);

	assert(rdsu_init_from_arrays(&rdsu, 5, parents, ranks) == &rdsu);
	assert(rdsu.size == 5);
	assert(rdsu.rp_single_alloc == false);
	assert(rdsu.parent == (uint16_t *)parents);
	assert(rdsu.rank == (uint16_t *)ranks);
	assert(rdsu.change_log == NULL);
	rdsu_destroy(&rdsu);
}
