#include <assert.h>

#include "cdsu_tests.h"

void test_cdsu_union(void) {
	t_cdsu *cdsu = cdsu_new(5);

	assert(cdsu_nb_components(*cdsu) == 5);
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_find(cdsu, 0) == cdsu_find(cdsu, 1));
	assert(cdsu_nb_components(*cdsu) == 4);
	cdsu_union(cdsu, 2, 3);
	cdsu_union(cdsu, 1, 3);
	assert(cdsu_find(cdsu, 0) == cdsu_find(cdsu, 2));
	assert(cdsu_nb_components(*cdsu) == 2);
	cdsu_free(cdsu);
}

void test_cdsu_find(void) {
	t_cdsu *cdsu = cdsu_new(5);
	assert(cdsu_find(cdsu, 0) == 0);
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_find(cdsu, 0) == cdsu_find(cdsu, 1));
	cdsu_free(cdsu);
}

void test_cdsu_strict_find(void) {
	t_cdsu *cdsu = cdsu_new(5);
	assert(cdsu_strict_find(*cdsu, 0) == 0);
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_strict_find(*cdsu, 0) == cdsu_strict_find(*cdsu, 1));
	cdsu_free(cdsu);
}

void test_cdsu_compress_all(void) {
	t_cdsu *cdsu = cdsu_new(5);
	cdsu_union(cdsu, 0, 1);
	cdsu_union(cdsu, 1, 2);
	cdsu_union(cdsu, 2, 3);

	cdsu->parent[0] = 1;
	cdsu->parent[1] = 2;
	cdsu->parent[2] = 3;
	cdsu->parent[3] = 3;

	assert(cdsu_compress_all(cdsu) == 5);
	assert(cdsu->parent[0] == 2);
	assert(cdsu->parent[1] == 3);
	assert(cdsu->parent[2] == 3);
	cdsu_free(cdsu);
}
