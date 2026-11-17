#include <assert.h>

#include "rdsu_tests.h"

NO_COV void test_cm_rdsu_max_depth(void) {
	t_cm_rdsu *rdsu = rdsu_new(5);

	assert(rdsu_max_depth(rdsu) == 0);
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_max_depth(rdsu) == 1);
	// chain 4->3->2->1->0: real depth (4) is way above the max rank (1)
	rdsu->parent[1] = 0;
	rdsu->parent[2] = 1;
	rdsu->parent[3] = 2;
	rdsu->parent[4] = 3;
	assert(rdsu_max_depth(rdsu) == 4);
	rdsu_free(&rdsu);
	assert(rdsu_max_depth(rdsu) == -1);
}
NO_COV void test_cm_rdsu_size(void) {
	t_cm_rdsu *rdsu = rdsu_new(12);

	assert(rdsu_size(rdsu) == 12);
	rdsu_destroy(rdsu);
	assert(rdsu_size(rdsu) == 0);
	rdsu_free(&rdsu);
	assert(rdsu_size(rdsu) == 0);
}
NO_COV void test_cm_rdsu_reserved_logs_nb(void) {
	t_cm_rdsu *rdsu = rdsu_new(5);

	assert(rdsu_reserved_logs_nb(rdsu) == 0); // empty rdsu
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_reserved_logs_nb(rdsu) == 0);
	rdsu_rewind(rdsu);
	assert(rdsu_reserved_logs_nb(rdsu) == 1);
	rdsu_free(&rdsu);
	assert(rdsu_reserved_logs_nb(rdsu) == 0);
}
NO_COV void test_cm_rdsu_nb_components(void) {
	t_cm_rdsu *rdsu = rdsu_new(5);

	assert(rdsu_nb_components(rdsu) == 5);
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_nb_components(rdsu) == 4);
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_nb_components(rdsu) == 4);
	rdsu_rewind(rdsu);
	assert(rdsu_nb_components(rdsu) == 5);
	rdsu_free(&rdsu);
	assert(rdsu_nb_components(rdsu) == 0);
}
