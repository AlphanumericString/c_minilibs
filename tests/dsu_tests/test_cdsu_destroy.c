#include "cdsu_tests.h"

// destroy
void test_cdsu_clr(void) {
	t_cdsu *cdsu     = cdsu_new(10);
	cdsu->parent[0]  = 5;
	cdsu->rank[0]    = 2;
	cdsu->components = 4;

	cdsu_clr(cdsu);
	assert(cdsu->parent[0] == 0);
	assert(cdsu->rank[0] == 0);
	assert(cdsu->components == 10);
	cdsu_free(cdsu);
}

void test_cdsu_destroy(void) {
	t_cdsu cdsu;
	cdsu_init(&cdsu, 10);
	cdsu_destroy(&cdsu);
	assert(cdsu.parent == NULL);
	assert(cdsu.size == 0);
}

void test_cdsu_free(void) {
	t_cdsu *d = cdsu_new(10);

	assert(d != NULL);
	cdsu_free(d); // check w valgrind or other leakcheck tool
}
