#include <assert.h>

#include "rdsu_tests.h"

NO_COV void test_rdsu_max_depth(void) {
	t_rdsu *rdsu = rdsu_new(5);

	assert(rdsu_max_depth(*rdsu) == 0);
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_max_depth(*rdsu) == 1);
	rdsu_free(&rdsu);
}
NO_COV void test_rdsu_size(void) {
	t_rdsu *rdsu = rdsu_new(12);

	assert(rdsu_size(*rdsu) == 12);
	rdsu_destroy(rdsu);
	assert(rdsu_size(*rdsu) == 0);
	rdsu_free(&rdsu);
}
NO_COV void test_rdsu_nb_components(void) {
	t_rdsu *rdsu = rdsu_new(5);

	assert(rdsu_nb_components(*rdsu) == 5);
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_nb_components(*rdsu) == 4);
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_nb_components(*rdsu) == 4);
	rdsu_rewind(rdsu);
	assert(rdsu_nb_components(*rdsu) == 5);
	rdsu_free(&rdsu);
}
