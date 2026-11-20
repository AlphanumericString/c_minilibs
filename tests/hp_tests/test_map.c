#include <assert.h>
#include <stdlib.h>

#include "hp_tests.h"

static void build_min(t_cm_hp *h) {
	int vals[] = {1, 2, 3, 4, 5};

	for (size_t i = 0; i < 5; i++)
		assert(hp_min_push(h, &vals[i]) == true);
}

NO_COV void test_cm_hp_map(void) {
	t_cm_hp h;
	void  **out;

	assert(hp_init(&h, 5, sizeof(int), cb_cmp_int) == &h);
	build_min(&h);

	out = hp_map(&h, cb_map_identity);
	assert(out != NULL);
	for (size_t i = 0; i < h.nb; i++) {
		assert(out[i] == hp_at(&h, i));
		assert(*(int *)out[i] == (int)(i + 1));
	}
	free(out);

	assert(hp_map(&h, cb_map_fail) == NULL);

	hp_clear(&h);
	assert(hp_map(&h, cb_map_identity) == NULL);

	assert(hp_map(NULL, cb_map_identity) == NULL);
	assert(hp_map(&h, NULL) == NULL);
	assert(hp_map(&(t_cm_hp){0}, cb_map_identity) == NULL);

	build_min(&h);
	alloc_reset_counter();
	alloc_set_call_limit(0);
	assert(hp_map(&h, cb_map_identity) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	hp_delete(&h);
}
