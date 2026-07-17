#include "rdsu_tests.h"

// querry
void test_rdsu_is_related(void) {
	t_rdsu *rdsu = rdsu_new(5);
	assert(!rdsu_is_related(rdsu, 0, 1));
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_is_related(rdsu, 0, 1));
	rdsu_free(rdsu);
}

void test_rdsu_validate(void) {
	t_rdsu *rdsu = rdsu_new(5);
	assert(rdsu_validate(*rdsu));
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_validate(*rdsu));

	// Create cycle to invalidate
	rdsu->parent[0] = 1;
	rdsu->parent[1] = 0;
	assert(!rdsu_validate(*rdsu));

	// Restore validity for free
	rdsu->parent[0] = 0;
	rdsu->parent[1] = 1;
	rdsu_free(rdsu);
}
