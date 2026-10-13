#include <assert.h>
#include <stdbool.h>

#include "cdsu_tests.h"

void test_cdsu_is_related(void) {
	t_cdsu *cdsu = cdsu_new(5);
	assert(!cdsu_is_related(cdsu, 0, 1));
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_is_related(cdsu, 0, 1));
	assert(cdsu_is_related(NULL, 0, 1) == false);
	assert(cdsu_is_related(cdsu, 99, 1) == false);
	assert(cdsu_is_related(cdsu, 0, 99) == false);
	assert(cdsu_is_related(cdsu, 99, 99) == false);
	cdsu_free(&cdsu);
}

void test_cdsu_strict_is_related(void) {
	t_cdsu *cdsu = cdsu_new(5);
	assert(!cdsu_strict_is_related(*cdsu, 0, 1));
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_strict_is_related(*cdsu, 0, 1));
	assert(cdsu_strict_is_related(*cdsu, 99, 1) == false);
	assert(cdsu_strict_is_related(*cdsu, 0, 99) == false);
	assert(cdsu_strict_is_related(*cdsu, 99, 99) == false);
	cdsu_free(&cdsu);
}

void test_cdsu_validate(void) {
	t_cdsu *cdsu = cdsu_new(5);
	t_cdsu  intact_copy;

	assert(cdsu_validate(*cdsu));
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_validate(*cdsu));
	cdsu_union(cdsu, 2, 3);
	cdsu_union(cdsu, 3, 4);
	cdsu_union(cdsu, 1, 2);
	assert(cdsu_validate(*cdsu));

	cdsu->parent[0] = 1;
	cdsu->parent[1] = 0;
	assert(!cdsu_validate(*cdsu));
	cdsu->parent[1] = 0;
	cdsu->parent[0] = 2;
	cdsu->parent[2] = 1;
	assert(!cdsu_validate(*cdsu));
	cdsu->parent[0] = 1;
	cdsu->parent[1] = 2;
	cdsu->parent[2] = 3;
	cdsu->parent[3] = 4;
	cdsu->parent[4] = 4;
	assert(cdsu_validate(*cdsu) == true);
	cdsu->parent[4] = 0;
	assert(cdsu_validate(*cdsu) == false);
	cdsu->parent[4] = 4;
	cdsu->rank[0]   = cdsu_size(*cdsu) * 2;
	assert(cdsu_validate(*cdsu) == false);
	cdsu->rank[0] = 4;
	assert(cdsu_validate(*cdsu) == true);
	cdsu->parent[0] = cdsu_size(*cdsu) * 2;
	assert(cdsu_validate(*cdsu) == false);
	cdsu->parent[0] = 1;
	assert(cdsu_validate(*cdsu) == true);
	cdsu->parent[2] = cdsu_size(*cdsu) * 2;
	assert(cdsu_validate(*cdsu) == false);
	cdsu->parent[2] = 3;
	assert(cdsu_validate(*cdsu) == true);

	intact_copy  = *cdsu;
	cdsu->parent = NULL;
	assert(cdsu_validate(*cdsu) == false);
	*cdsu      = intact_copy;
	cdsu->rank = NULL;
	assert(cdsu_validate(*cdsu) == false);
	*cdsu      = intact_copy;
	cdsu->size = 0;
	assert(cdsu_validate(*cdsu) == false);

	cdsu_free(&cdsu);
}
