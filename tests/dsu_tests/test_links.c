#include "dsu_tests.h"

// links
void test_dsu_union(void) {
	t_dsu *dsu = dsu_new(5);

	assert(dsu_nb_components(*dsu) == 5);
	dsu_union(dsu, 0, 1);
	assert(dsu_find(dsu, 0) == dsu_find(dsu, 1));
	assert(dsu_nb_components(*dsu) == 4);
	dsu_union(dsu, 2, 3);
	dsu_union(dsu, 1, 3);
	assert(dsu_find(dsu, 0) == dsu_find(dsu, 2));
	assert(dsu_nb_components(*dsu) == 2);
	dsu_free(dsu);
}

void test_dsu_find(void) {
	t_dsu *dsu = dsu_new(5);
	assert(dsu_find(dsu, 0) == 0);
	dsu_union(dsu, 0, 1);
	assert(dsu_find(dsu, 0) == dsu_find(dsu, 1));
	dsu_free(dsu);
}

void test_dsu_strict_find(void) {
	t_dsu *dsu = dsu_new(5);
	assert(dsu_strict_find(*dsu, 0) == 0);
	dsu_union(dsu, 0, 1);
	assert(dsu_strict_find(*dsu, 0) == dsu_strict_find(*dsu, 1));
	dsu_free(dsu);
}

void test_dsu_compress_all(void) {
	t_dsu *dsu = dsu_new(5);
	dsu_union(dsu, 0, 1);
	dsu_union(dsu, 1, 2);
	dsu_union(dsu, 2, 3);

	// Create a long uncompressed chain to test compress_all
	dsu->parent[0] = 1;
	dsu->parent[1] = 2;
	dsu->parent[2] = 3;
	dsu->parent[3] = 3;

	// Path halving will make parent[0]=2, parent[1]=3, parent[2]=3
	assert(dsu_compress_all(dsu) == 5);
	assert(dsu->parent[0] == 2);
	assert(dsu->parent[1] == 3);
	assert(dsu->parent[2] == 3);
	dsu_free(dsu);
}
