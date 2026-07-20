#include <assert.h>

#include "rdsu_tests.h"

void test_rdsu_clr(void) {
	t_rdsu *rdsu = rdsu_new(10);

	rdsu->parent[0]  = 5;
	rdsu->rank[0]    = 2;
	rdsu->components = 4;
	rdsu_union(rdsu, 2, 4);
	rdsu_clr(rdsu);
	assert(rdsu->parent[0] == 0);
	assert(rdsu->rank[0] == 0);
	assert(rdsu->components == 10);
	assert(rdsu->change_log == NULL);
	rdsu_free(rdsu);
}
void test_rdsu_destroy(void) {
	t_rdsu rdsu;

	rdsu_init(&rdsu, 10);
	rdsu_union(&rdsu, 2, 0);
	rdsu_destroy(&rdsu);
	assert(rdsu.parent == NULL);
	assert(rdsu.size == 0);
	assert(rdsu.change_log == NULL);
}
void test_rdsu_free(void) {
	t_rdsu *d = rdsu_new(10);

	assert(d != NULL);
	rdsu_free(d);
}
