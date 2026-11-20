#include <assert.h>
#include <string.h>

#include "hp_tests.h"

NO_COV void test_cm_hp_min_heapify(void) {
	int     in[]  = {5, 1, 4, 2, 8, 0};
	int     ord[] = {0, 1, 2, 4, 5, 8};
	t_cm_hp h;
	int     out;

	assert(hp_init(&h, 6, sizeof(int), cb_cmp_int) == &h);
	memcpy(h.elems, in, sizeof(in));
	h.nb = 6;
	assert(hp_min_heapify(&h) == true);
	assert(hp_min_is_valid(&h) == true);
	for (size_t i = 0; i < 6; i++) {
		assert(hp_min_pop(&h, &out) == true);
		assert(out == ord[i]);
	}

	hp_clear(&h);
	assert(hp_min_heapify(&h) == true);
	assert(hp_min_push(&h, &ord[0]) == true);
	assert(hp_min_heapify(&h) == true);
	assert(hp_min_is_valid(&h) == true);
	hp_delete(&h);

	assert(hp_min_heapify(NULL) == false);
	assert(hp_min_heapify(&(t_cm_hp){0}) == false);
}

NO_COV void test_cm_hp_max_heapify(void) {
	int     in[]  = {5, 1, 4, 2, 8, 0};
	int     ord[] = {8, 5, 4, 2, 1, 0};
	t_cm_hp h;
	int     out;

	assert(hp_init(&h, 6, sizeof(int), cb_cmp_int) == &h);
	memcpy(h.elems, in, sizeof(in));
	h.nb = 6;
	assert(hp_max_heapify(&h) == true);
	assert(hp_max_is_valid(&h) == true);
	for (size_t i = 0; i < 6; i++) {
		assert(hp_max_pop(&h, &out) == true);
		assert(out == ord[i]);
	}

	hp_clear(&h);
	assert(hp_max_heapify(&h) == true);
	assert(hp_max_push(&h, &ord[0]) == true);
	assert(hp_max_heapify(&h) == true);
	assert(hp_max_is_valid(&h) == true);
	hp_delete(&h);

	assert(hp_max_heapify(NULL) == false);
	assert(hp_max_heapify(&(t_cm_hp){0}) == false);
}
