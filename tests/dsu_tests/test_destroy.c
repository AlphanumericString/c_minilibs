#include <dsu/disjointed_set_union.h>

#include "dsu_tests.h"

// destroy
void test_dsu_clr(void) {
	t_dsu *dsu      = dsu_new(10);
	dsu->parent[0]  = 5;
	dsu->rank[0]    = 2;
	dsu->components = 4;

	dsu_clr(dsu);
	assert(dsu->parent[0] == 0);
	assert(dsu->rank[0] == 0);
	assert(dsu->components == 10);
	dsu_free(dsu);
}

void test_dsu_destroy(void) {
	t_dsu dsu;
	dsu_init(&dsu, 10);
	dsu_destroy(&dsu);
	assert(dsu.parent == NULL);
	assert(dsu.size == 0);
}

void test_dsu_free(void) {
	t_dsu *d = dsu_new(10);

	assert(d != NULL);
	dsu_free(d); // check w valgrind or other leakcheck tool
}
