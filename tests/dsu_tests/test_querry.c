#include "dsu_tests.h"

// querry
void test_dsu_is_related(void) {
	t_dsu *dsu = dsu_new(5);
	assert(!dsu_is_related(dsu, 0, 1));
	dsu_union(dsu, 0, 1);
	assert(dsu_is_related(dsu, 0, 1));
	dsu_free(dsu);
}

void test_dsu_strict_is_related(void) {
	t_dsu *dsu = dsu_new(5);
	assert(!dsu_strict_is_related(*dsu, 0, 1));
	dsu_union(dsu, 0, 1);
	assert(dsu_strict_is_related(*dsu, 0, 1));
	dsu_free(dsu);
}

void test_dsu_validate(void) {
	t_dsu *dsu = dsu_new(5);
	assert(dsu_validate(*dsu));
	dsu_union(dsu, 0, 1);
	assert(dsu_validate(*dsu));

	// Create cycle to invalidate
	dsu->parent[0] = 1;
	dsu->parent[1] = 0;
	assert(!dsu_validate(*dsu));

	// Restore validity for free
	dsu->parent[0] = 0;
	dsu->parent[1] = 1;
	dsu_free(dsu);
}
