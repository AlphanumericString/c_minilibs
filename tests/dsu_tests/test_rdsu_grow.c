#include <assert.h>

#include "rdsu_tests.h"

void test_rdsu_grow(void) {
	t_rdsu *rdsu = rdsu_new(5);

	assert(rdsu->size == 5);
	rdsu_grow(rdsu, 5);
	assert(rdsu->size == 10);
	assert(rdsu->components == 10);
	for (size_t i = 0; i < 10; i++)
		assert(rdsu->parent[i] == i);
	rdsu_free(rdsu);
}
