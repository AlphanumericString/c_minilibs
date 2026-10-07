#include <assert.h>
#include <stdlib.h>

#include "cdsu_tests.h"

void test_cdsu_grow(void) {
	t_cdsu *cdsu = cdsu_new(5);
	t_cdsu  malicious;

	assert(cdsu->size == 5);
	assert(cdsu_grow(cdsu, 5) == true);
	assert(cdsu->size == 10);
	assert(cdsu->components == 10);
	for (size_t i = 0; i < 10; i++) {
		assert(cdsu->parent[i] == i);
	}
	assert(cdsu_grow(cdsu, 0) == true);
	assert(cdsu->size == 10);
	assert(cdsu->components == 10);
	for (size_t i = 0; i < 10; i++) {
		assert(cdsu->parent[i] == i);
	}
	cdsu_free(&cdsu);

	void *buf_parents = malloc(sizeof(int) * 5);
	void *buf_rank    = malloc(sizeof(int) * 5);
	cdsu              = cdsu_new_from_array(5, buf_parents, buf_rank);
	assert(cdsu != NULL && cdsu->size == 5 && cdsu->components == 5);
	assert(cdsu->rp_single_alloc == false);
	for (size_t i = 0; i < 5; i++) {
		assert(cdsu->parent[i] == i);
	}
	cdsu_grow(cdsu, 5);
	assert(cdsu != NULL && cdsu->size == 10 && cdsu->components == 10);
	assert(cdsu->rp_single_alloc == true);
	for (size_t i = 0; i < 10; i++) {
		assert(cdsu->parent[i] == i);
	}
	cdsu_free(&cdsu);

	malicious        = (t_cdsu){.components      = 10,
	                            .parent          = (void *)((long)rand()),
	                            .rank            = (void *)((long)rand()),
	                            .rp_single_alloc = false,
	                            .size            = 10};
	malicious.parent = NULL;
	assert(cdsu_grow(&malicious, 5) == false);
	malicious.parent = (void *)((long)rand());
	malicious.rank   = NULL;
	assert(cdsu_grow(&malicious, 5) == false);
	malicious.rank = (void *)((long)rand());
	malicious.size = 0;
	assert(cdsu_grow(&malicious, 5) == false);
	assert(cdsu_grow(NULL, 5) == false);
}
