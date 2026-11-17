#include <assert.h>

#include "cdsu_tests.h"

NO_COV void test_cm_cdsu_max_depth(void) {
	t_cm_cdsu *cdsu = cdsu_new(5);

	assert(cdsu_max_depth(cdsu) == 0);
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_max_depth(cdsu) == 1);
	// chain 4->3->2->1->0: real depth (4) is way above the max rank (1)
	cdsu->parent[1] = 0;
	cdsu->parent[2] = 1;
	cdsu->parent[3] = 2;
	cdsu->parent[4] = 3;
	assert(cdsu_max_depth(cdsu) == 4);
	cdsu_free(&cdsu);
	assert(cdsu_max_depth(cdsu) == -1);
}

NO_COV void test_cm_cdsu_size(void) {
	t_cm_cdsu *cdsu = cdsu_new(12);

	assert(cdsu_size(cdsu) == 12);
	cdsu_free(&cdsu);
	assert(cdsu_size(cdsu) == 0);
}

NO_COV void test_cm_cdsu_nb_components(void) {
	t_cm_cdsu *cdsu = cdsu_new(5);

	assert(cdsu_nb_components(cdsu) == 5);
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_nb_components(cdsu) == 4);
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_nb_components(cdsu) == 4);
	cdsu_union(cdsu, 3, 2);
	assert(cdsu_nb_components(cdsu) == 3);
	cdsu_union(cdsu, 4, 3);
	assert(cdsu_nb_components(cdsu) == 2);
	cdsu->components = 42;
	assert(cdsu_nb_components(cdsu) == 42);

	cdsu_free(&cdsu);
	assert(cdsu_nb_components(cdsu) == 0);
}
