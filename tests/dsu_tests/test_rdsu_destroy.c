#include <assert.h>

#include "rdsu_tests.h"

NO_COV void test_cm_rdsu_clr(void) {
	t_cm_rdsu *rdsu = rdsu_new(10);
	t_cm_rdsu  intact_copy;

	assert(rdsu_clr(rdsu) == true); // already clean
	rdsu->parent[0]  = 5;
	rdsu->rank[0]    = 2;
	rdsu->components = 4;
	rdsu_union(rdsu, 2, 4);
	assert(rdsu->change_log != NULL);
	assert(rdsu_clr(rdsu) == true);
	assert(rdsu_clr(rdsu) == true);
	assert(rdsu->parent[0] == 0);
	assert(rdsu->rank[0] == 0);
	assert(rdsu->components == 10);
	assert(rdsu->change_log == NULL);
	intact_copy  = *rdsu;
	rdsu->parent = NULL;
	assert(rdsu_clr(rdsu) == false);
	*rdsu      = intact_copy;
	rdsu->rank = NULL;
	assert(rdsu_clr(rdsu) == false);
	assert(rdsu_clr(NULL) == false);

	*rdsu = intact_copy;
	rdsu_free(&rdsu);
}
NO_COV void test_cm_rdsu_destroy(void) {
	t_cm_rdsu rdsu;

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
NO_COV void test_cm_rdsu_free(void) {
	t_cm_rdsu *d = rdsu_new(10);

	assert(d != NULL);
	rdsu_free(&d);
	assert(d == NULL);
	rdsu_free(NULL);
}
