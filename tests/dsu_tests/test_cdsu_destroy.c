#include <assert.h>

#include "cdsu_tests.h"

NO_COV void test_cdsu_clr(void) {
	t_cdsu *cdsu     = cdsu_new(10);
	cdsu->parent[0]  = 5;
	cdsu->rank[0]    = 2;
	cdsu->components = 4;

	assert(cdsu_clr(cdsu) == true);
	assert(cdsu_clr(cdsu) == true);
	assert(cdsu_clr(cdsu) == true);
	assert(cdsu_clr(cdsu) == true);
	assert(cdsu->parent[0] == 0);
	assert(cdsu->rank[0] == 0);
	assert(cdsu->components == 10);
	assert(cdsu_clr(NULL) == false);
	cdsu_free(&cdsu);
}

NO_COV void test_cdsu_destroy(void) {
	t_cdsu cdsu;
	cdsu_init(&cdsu, 10);
	cdsu_destroy(&cdsu);
	assert(cdsu.parent == NULL);
	assert(cdsu.size == 0);
	cdsu_destroy(&cdsu);
	cdsu_destroy(NULL);
}

NO_COV void test_cdsu_free(void) {
	t_cdsu *d = cdsu_new(10);

	assert(d != NULL);
	cdsu_free(&d);
	assert(d == NULL);
	cdsu_free(&d);
	assert(d == NULL);
	cdsu_free(NULL);
}
