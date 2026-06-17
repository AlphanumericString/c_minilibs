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

	dsu_destroy(dsu);
}
void test_dsu_new_from_array(void) {}
void test_dsu_init(void) {}
void test_dsu_init_from_arrays(void) {}
