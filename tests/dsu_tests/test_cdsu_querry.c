#include "cdsu_tests.h"

// querry
void test_cdsu_is_related(void) {
	t_cdsu *cdsu = cdsu_new(5);
	assert(!cdsu_is_related(cdsu, 0, 1));
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_is_related(cdsu, 0, 1));
	cdsu_free(cdsu);
}

void test_cdsu_strict_is_related(void) {
	t_cdsu *cdsu = cdsu_new(5);
	assert(!cdsu_strict_is_related(*cdsu, 0, 1));
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_strict_is_related(*cdsu, 0, 1));
	cdsu_free(cdsu);
}

void test_cdsu_validate(void) {
	t_cdsu *cdsu = cdsu_new(5);
	assert(cdsu_validate(*cdsu));
	cdsu_union(cdsu, 0, 1);
	assert(cdsu_validate(*cdsu));

	// Create cycle to invalidate
	cdsu->parent[0] = 1;
	cdsu->parent[1] = 0;
	assert(!cdsu_validate(*cdsu));

	// Restore validity for free
	cdsu->parent[0] = 0;
	cdsu->parent[1] = 1;
	cdsu_free(cdsu);
}
