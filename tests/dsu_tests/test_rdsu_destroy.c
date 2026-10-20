#include <assert.h>

#include "rdsu_tests.h"

NO_COV void test_rdsu_clr(void) {
	t_rdsu *rdsu = rdsu_new(10);
	t_rdsu  intact_copy;

	rdsu->parent[0]  = 5;
	rdsu->rank[0]    = 2;
	rdsu->components = 4;
	rdsu_union(rdsu, 2, 4);
	assert(rdsu->change_log != NULL);
	rdsu_clr(rdsu);
	assert(rdsu->parent[0] == 0);
	assert(rdsu->rank[0] == 0);
	assert(rdsu->components == 10);
	assert(rdsu->change_log == NULL);
	intact_copy  = *rdsu;
	rdsu->parent = NULL;
	rdsu_clr(rdsu);
	*rdsu      = intact_copy;
	rdsu->rank = NULL;
	rdsu_clr(rdsu);
	rdsu_clr(NULL);

	*rdsu = intact_copy;
	rdsu_free(&rdsu);
}
NO_COV void test_rdsu_destroy(void) {
	t_rdsu rdsu;

	rdsu_init(&rdsu, 10);
	rdsu_union(&rdsu, 2, 0);
	rdsu_destroy(&rdsu);
	assert(rdsu.parent == NULL);
	assert(rdsu.size == 0);
	assert(rdsu.change_log == NULL);
	rdsu_destroy(&rdsu);
	rdsu_destroy(&rdsu);
	rdsu_destroy(NULL);
}
NO_COV void test_rdsu_free(void) {
	t_rdsu *d = rdsu_new(10);

	assert(d != NULL);
	rdsu_free(&d);
	assert(d == NULL);
	rdsu_free(NULL);
}
