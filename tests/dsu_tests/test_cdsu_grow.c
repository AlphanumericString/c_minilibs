#include <assert.h>

#include "cdsu_tests.h"

void test_cdsu_grow(void) {
	t_cdsu *cdsu = cdsu_new(5);

	assert(cdsu->size == 5);
	cdsu_grow(cdsu, 5);
	assert(cdsu->size == 10);
	assert(cdsu->components == 10);
	for (size_t i = 0; i < 10; i++) {
		assert(cdsu->parent[i] == i);
	}
	cdsu_free(cdsu);
}
