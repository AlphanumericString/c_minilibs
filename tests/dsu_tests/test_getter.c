#include "dsu_tests.h"

// getter
void test_dsu_max_depth(void) {
	t_dsu *dsu = dsu_new(5);
	assert(dsu_max_depth(*dsu) == 0);
	dsu_union(dsu, 0, 1);
	assert(dsu_max_depth(*dsu) == 1);
	dsu_free(dsu);
}

void test_dsu_size(void) {
	t_dsu *dsu = dsu_new(12);
	assert(dsu_size(*dsu) == 12);
	dsu_free(dsu);
}

void test_dsu_nb_components(void) {
	t_dsu *dsu = dsu_new(5);
	assert(dsu_nb_components(*dsu) == 5);
	dsu_union(dsu, 0, 1);
	assert(dsu_nb_components(*dsu) == 4);
	dsu_union(dsu, 0, 1); // Already unioned
	assert(dsu_nb_components(*dsu) == 4);
	dsu_free(dsu);
}
