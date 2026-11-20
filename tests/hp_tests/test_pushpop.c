#include <assert.h>

#include "hp_tests.h"

static void build_min(t_cm_hp *h) {
	int vals[] = {5, 10, 15};

	for (size_t i = 0; i < 3; i++)
		assert(hp_min_push(h, &vals[i]) == true);
}

static void build_max(t_cm_hp *h) {
	int vals[] = {5, 10, 15};

	for (size_t i = 0; i < 3; i++)
		assert(hp_max_push(h, &vals[i]) == true);
}

static void drain_min(t_cm_hp *h, const int *expected, size_t n) {
	int out;

	for (size_t i = 0; i < n; i++) {
		assert(hp_min_pop(h, &out) == true);
		assert(out == expected[i]);
	}
	assert(hp_min_pop(h, &out) == false);
}

static void drain_max(t_cm_hp *h, const int *expected, size_t n) {
	int out;

	for (size_t i = 0; i < n; i++) {
		assert(hp_max_pop(h, &out) == true);
		assert(out == expected[i]);
	}
	assert(hp_max_pop(h, &out) == false);
}

NO_COV void test_cm_hp_min_pushpop(void) {
	t_cm_hp h;
	int     out;
	int     ord1[] = {5, 10, 15};
	int     ord2[] = {7, 10, 15};
	int     ord3[] = {10, 15, 20};

	assert(hp_init(&h, 4, sizeof(int), cb_cmp_int) == &h);
	assert(hp_min_pushpop(&h, &(int){42}, &out) == true);
	assert(out == 42);
	assert(h.nb == 0);
	assert(hp_min_is_valid(&h) == true);
	assert(hp_min_replace(&h, &(int){42}, &out) == false);

	build_min(&h);
	assert(hp_min_pushpop(&h, &(int){3}, &out) == true);
	assert(out == 3);
	assert(h.nb == 3);
	assert(hp_min_is_valid(&h) == true);
	drain_min(&h, ord1, 3);

	build_min(&h);
	assert(hp_min_pushpop(&h, &(int){5}, &out) == true);
	assert(out == 5);
	assert(h.nb == 3);
	assert(hp_min_is_valid(&h) == true);
	drain_min(&h, ord1, 3);

	build_min(&h);
	assert(hp_min_pushpop(&h, &(int){7}, &out) == true);
	assert(out == 5);
	assert(h.nb == 3);
	assert(hp_min_is_valid(&h) == true);
	drain_min(&h, ord2, 3);

	build_min(&h);
	assert(hp_min_pushpop(&h, &(int){20}, &out) == true);
	assert(out == 5);
	assert(h.nb == 3);
	assert(hp_min_is_valid(&h) == true);
	drain_min(&h, ord3, 3);

	assert(hp_min_pushpop(NULL, &(int){1}, &out) == false);
	assert(hp_min_pushpop(&h, NULL, &out) == false);
	assert(hp_min_pushpop(&h, &(int){1}, NULL) == false);
	h.sz_e = 0;
	assert(hp_min_pushpop(&h, &(int){1}, &out) == false);
	h.sz_e = sizeof(int);

	hp_delete(&h);
}

NO_COV void test_cm_hp_max_pushpop(void) {
	t_cm_hp h;
	int     out;
	int     ord1[] = {15, 10, 5};
	int     ord2[] = {12, 10, 5};
	int     ord3[] = {10, 5, 0};

	assert(hp_init(&h, 4, sizeof(int), cb_cmp_int) == &h);
	assert(hp_max_pushpop(&h, &(int){42}, &out) == true);
	assert(out == 42);
	assert(h.nb == 0);
	assert(hp_max_is_valid(&h) == true);
	assert(hp_max_replace(&h, &(int){42}, &out) == false);

	build_max(&h);
	assert(hp_max_pushpop(&h, &(int){20}, &out) == true);
	assert(out == 20);
	assert(h.nb == 3);
	assert(hp_max_is_valid(&h) == true);
	drain_max(&h, ord1, 3);

	build_max(&h);
	assert(hp_max_pushpop(&h, &(int){15}, &out) == true);
	assert(out == 15);
	assert(h.nb == 3);
	assert(hp_max_is_valid(&h) == true);
	drain_max(&h, ord1, 3);

	build_max(&h);
	assert(hp_max_pushpop(&h, &(int){12}, &out) == true);
	assert(out == 15);
	assert(h.nb == 3);
	assert(hp_max_is_valid(&h) == true);
	drain_max(&h, ord2, 3);

	build_max(&h);
	assert(hp_max_pushpop(&h, &(int){0}, &out) == true);
	assert(out == 15);
	assert(h.nb == 3);
	assert(hp_max_is_valid(&h) == true);
	drain_max(&h, ord3, 3);

	assert(hp_max_pushpop(NULL, &(int){1}, &out) == false);
	assert(hp_max_pushpop(&h, NULL, &out) == false);
	assert(hp_max_pushpop(&h, &(int){1}, NULL) == false);
	h.sz_e = 0;
	assert(hp_max_pushpop(&h, &(int){1}, &out) == false);
	h.sz_e = sizeof(int);

	hp_delete(&h);
}

NO_COV void test_cm_hp_min_replace(void) {
	t_cm_hp h;
	int     out;
	int     ord1[] = {7, 10, 15};
	int     ord2[] = {1, 10, 15};

	assert(hp_init(&h, 4, sizeof(int), cb_cmp_int) == &h);
	assert(hp_min_replace(&h, &(int){42}, &out) == false);

	build_min(&h);
	assert(hp_min_replace(&h, &(int){7}, &out) == true);
	assert(out == 5);
	assert(h.nb == 3);
	assert(hp_min_is_valid(&h) == true);
	drain_min(&h, ord1, 3);

	build_min(&h);
	assert(hp_min_replace(&h, &(int){1}, &out) == true);
	assert(out == 5);
	assert(h.nb == 3);
	assert(hp_min_is_valid(&h) == true);
	drain_min(&h, ord2, 3);

	assert(hp_min_replace(NULL, &(int){1}, &out) == false);
	assert(hp_min_replace(&h, NULL, &out) == false);
	assert(hp_min_replace(&h, &(int){1}, NULL) == false);
	build_min(&h);
	h.sz_e = 0;
	assert(hp_min_replace(&h, &(int){1}, &out) == false);
	h.sz_e = sizeof(int);

	hp_delete(&h);
}

NO_COV void test_cm_hp_max_replace(void) {
	t_cm_hp h;
	int     out;
	int     ord1[] = {12, 10, 5};
	int     ord2[] = {20, 10, 5};

	assert(hp_init(&h, 4, sizeof(int), cb_cmp_int) == &h);
	assert(hp_max_replace(&h, &(int){42}, &out) == false);

	build_max(&h);
	assert(hp_max_replace(&h, &(int){12}, &out) == true);
	assert(out == 15);
	assert(h.nb == 3);
	assert(hp_max_is_valid(&h) == true);
	drain_max(&h, ord1, 3);

	build_max(&h);
	assert(hp_max_replace(&h, &(int){20}, &out) == true);
	assert(out == 15);
	assert(h.nb == 3);
	assert(hp_max_is_valid(&h) == true);
	drain_max(&h, ord2, 3);

	assert(hp_max_replace(NULL, &(int){1}, &out) == false);
	assert(hp_max_replace(&h, NULL, &out) == false);
	assert(hp_max_replace(&h, &(int){1}, NULL) == false);
	build_max(&h);
	h.sz_e = 0;
	assert(hp_max_replace(&h, &(int){1}, &out) == false);
	h.sz_e = sizeof(int);

	hp_delete(&h);
}
