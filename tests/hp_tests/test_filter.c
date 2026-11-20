#include <assert.h>

#include "hp_tests.h"

static void build_min(t_cm_hp *h) {
	int vals[] = {1, 2, 3, 4, 5};

	for (size_t i = 0; i < 5; i++)
		assert(hp_min_push(h, &vals[i]) == true);
}

static void build_max(t_cm_hp *h) {
	int vals[] = {1, 2, 3, 4, 5};

	for (size_t i = 0; i < 5; i++)
		assert(hp_max_push(h, &vals[i]) == true);
}

NO_COV void test_cm_hp_min_filter(void) {
	t_cm_hp h;
	int     out;
	size_t  removed;

	assert(hp_init(&h, 5, sizeof(int), cb_cmp_int) == &h);
	build_min(&h);

	assert(hp_min_filter(&h, cb_keep_even, NULL, NULL) == true);
	assert(h.nb == 2);
	assert(hp_min_is_valid(&h) == true);
	assert(hp_min_pop(&h, &out) == true);
	assert(out == 2);
	assert(hp_min_pop(&h, &out) == true);
	assert(out == 4);

	hp_clear(&h);
	build_min(&h);
	assert(hp_min_filter(&h, cb_keep_all, NULL, NULL) == true);
	assert(h.nb == 5);
	assert(hp_min_is_valid(&h) == true);

	hp_clear(&h);
	build_min(&h);
	assert(hp_min_filter(&h, cb_keep_none, NULL, NULL) == true);
	assert(h.nb == 0);
	assert(hp_min_is_valid(&h) == true);

	hp_clear(&h);
	build_min(&h);
	removed = 0;
	assert(hp_min_filter(&h, cb_keep_even, &removed, cb_del_count) == true);
	assert(h.nb == 2);
	assert(removed == 3);

	hp_clear(&h);
	assert(hp_min_filter(&h, cb_keep_all, NULL, NULL) == false);
	assert(hp_min_filter(NULL, cb_keep_all, NULL, NULL) == false);
	assert(hp_min_filter(&h, NULL, NULL, NULL) == false);

	assert(hp_min_push(&h, &(int){2}) == true);
	assert(hp_min_filter(&h, cb_keep_all, NULL, NULL) == true);
	assert(h.nb == 1);
	assert(hp_min_filter(&h, cb_keep_none, NULL, NULL) == true);
	assert(h.nb == 0);

	hp_delete(&h);
}

NO_COV void test_cm_hp_max_filter(void) {
	t_cm_hp h;
	int     out;
	size_t  removed;

	assert(hp_init(&h, 5, sizeof(int), cb_cmp_int) == &h);
	build_max(&h);

	assert(hp_max_filter(&h, cb_keep_even, NULL, NULL) == true);
	assert(h.nb == 2);
	assert(hp_max_is_valid(&h) == true);
	assert(hp_max_pop(&h, &out) == true);
	assert(out == 4);
	assert(hp_max_pop(&h, &out) == true);
	assert(out == 2);

	hp_clear(&h);
	build_max(&h);
	assert(hp_max_filter(&h, cb_keep_all, NULL, NULL) == true);
	assert(h.nb == 5);
	assert(hp_max_is_valid(&h) == true);

	hp_clear(&h);
	build_max(&h);
	assert(hp_max_filter(&h, cb_keep_none, NULL, NULL) == true);
	assert(h.nb == 0);
	assert(hp_max_is_valid(&h) == true);

	hp_clear(&h);
	build_max(&h);
	removed = 0;
	assert(hp_max_filter(&h, cb_keep_even, &removed, cb_del_count) == true);
	assert(h.nb == 2);
	assert(removed == 3);

	hp_clear(&h);
	assert(hp_max_filter(&h, cb_keep_all, NULL, NULL) == false);
	assert(hp_max_filter(NULL, cb_keep_all, NULL, NULL) == false);
	assert(hp_max_filter(&h, NULL, NULL, NULL) == false);

	assert(hp_max_push(&h, &(int){2}) == true);
	assert(hp_max_filter(&h, cb_keep_all, NULL, NULL) == true);
	assert(h.nb == 1);
	assert(hp_max_filter(&h, cb_keep_none, NULL, NULL) == true);
	assert(h.nb == 0);

	hp_delete(&h);
}
