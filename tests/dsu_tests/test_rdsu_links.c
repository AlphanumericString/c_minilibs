#include <assert.h>

#include "rdsu_tests.h"

void test_rdsu_union(void) {
	t_rdsu *rdsu = rdsu_new(5);

	assert(rdsu_nb_components(*rdsu) == 5);
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 1));
	assert(rdsu->change_log != NULL);
	assert(rdsu_nb_components(*rdsu) == 4);
	rdsu_rewind(rdsu);
	assert(rdsu_find(rdsu, 0) != rdsu_find(rdsu, 1));
	rdsu_union(rdsu, 0, 1); // 0-1 2 3 4
	rdsu_union(rdsu, 2, 3); // 0-1 2-3 4
	rdsu_union(rdsu, 1, 3); // 0-1-2-3 4
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 2));
	assert(rdsu_nb_components(*rdsu) == 2);
	rdsu_free(rdsu);
}

void test_rdsu_find(void) {
	t_rdsu *rdsu = rdsu_new(5);

	assert(rdsu_find(rdsu, 0) == 0);
	assert(rdsu_find(rdsu, 1) == 1);
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 1));
	rdsu_rewind(rdsu);
	assert(rdsu_find(rdsu, 0) != rdsu_find(rdsu, 1));
	rdsu_free(rdsu);
}

void test_rdsu_compress_all(void) {
	t_rdsu *rdsu = rdsu_new(5);

	rdsu_union(rdsu, 0, 1);
	rdsu_union(rdsu, 1, 2);
	rdsu_union(rdsu, 2, 3);
	rdsu->parent[0] = 1;
	rdsu->parent[1] = 2;
	rdsu->parent[2] = 3;
	rdsu->parent[3] = 3;
	assert(rdsu->change_log != NULL);
	assert(rdsu_compress_all(rdsu) == 5);
	assert(rdsu->change_log == NULL);
	assert(rdsu->parent[0] == 2);
	assert(rdsu->parent[1] == 3);
	assert(rdsu->parent[2] == 3);
	rdsu_free(rdsu);
}
