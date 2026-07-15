#include "dsu_tests.h"

// grow
void test_dsu_grow(void) {
	t_dsu *dsu = dsu_new(5);
	assert(dsu->size == 5);
	dsu_grow(dsu, 5);
	assert(dsu->size == 10);
	assert(dsu->components == 10);
	for (size_t i = 0; i < 10; i++) {
		assert(dsu->parent[i] == i);
	}
	dsu_free(dsu);
}
