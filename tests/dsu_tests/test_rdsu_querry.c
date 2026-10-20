#include <assert.h>

#include "rdsu_tests.h"

NO_COV void test_rdsu_is_related(void) {
	t_rdsu *rdsu = rdsu_new(5);
	assert(!rdsu_is_related(rdsu, 0, 1));
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_is_related(rdsu, 0, 1));

	assert(rdsu_is_related(NULL, 0, 99) == false);
	assert(rdsu_is_related(rdsu, 0, 99) == false);
	assert(rdsu_is_related(rdsu, 99, 1) == false);
	rdsu_free(&rdsu);
}

NO_COV void test_rdsu_validate(void) {
	t_rdsu *rdsu = rdsu_new(5);
	t_rdsu  intact_copy;

	assert(rdsu_validate(*rdsu));
	rdsu_union(rdsu, 0, 1);
	assert(rdsu_validate(*rdsu));

	rdsu->parent[0] = 1;
	rdsu->parent[1] = 0;
	assert(!rdsu_validate(*rdsu));
	rdsu->parent[0] = 1;
	rdsu->parent[1] = 2;
	rdsu->parent[2] = 3;
	rdsu->parent[3] = 4;
	rdsu->parent[4] = 0;
	assert(!rdsu_validate(*rdsu));
	rdsu->parent[4] = 4;
	assert(rdsu_validate(*rdsu) == true);
	rdsu->parent[4] = 99;
	assert(rdsu->size == 5);
	assert(!rdsu_validate(*rdsu));
	rdsu->parent[4] = 4;
	rdsu->parent[0] = 99;
	assert(!rdsu_validate(*rdsu));
	rdsu->parent[0] = 1;
	rdsu->rank[0]   = 99;
	assert(!rdsu_validate(*rdsu));
	rdsu->rank[0] = 0;
	rdsu->rank[2] = 99;
	assert(!rdsu_validate(*rdsu));
	rdsu->rank[2] = 0;

	intact_copy = *rdsu;

	rdsu->rank = NULL;
	assert(!rdsu_validate(*rdsu));
	*rdsu        = intact_copy;
	rdsu->parent = NULL;
	assert(!rdsu_validate(*rdsu));
	*rdsu      = intact_copy;
	rdsu->size = 0;
	assert(!rdsu_validate(*rdsu));
	*rdsu = intact_copy;

	rdsu_free(&rdsu);
}
