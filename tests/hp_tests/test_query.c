#include <assert.h>
#include <stdlib.h>

#include "hp_tests.h"

static void build_min(t_cm_hp *h) {
	int vals[] = {1, 2, 3, 4, 5};

	for (size_t i = 0; i < 5; i++)
		assert(hp_min_push(h, &vals[i]) == true);
}

NO_COV void test_cm_hp_peek(void) {
	t_cm_hp h;

	assert(hp_init(&h, 5, sizeof(int), cb_cmp_int) == &h);
	assert(hp_peek(&h) == NULL);
	build_min(&h);
	assert(hp_peek(&h) == hp_at(&h, 0));
	assert(*(int *)hp_peek(&h) == 1);
	assert(hp_peek(NULL) == NULL);
	hp_delete(&h);
}

NO_COV void test_cm_hp_find(void) {
	t_cm_hp h;
	int     search;

	assert(hp_init(&h, 5, sizeof(int), cb_cmp_int) == &h);
	search = 42;
	assert(hp_find(&h, cb_cmp_int, &search) == NULL);
	build_min(&h);

	search = 1;
	assert(hp_find(&h, cb_cmp_int, &search) == hp_at(&h, 0));
	search = 3;
	assert(hp_find(&h, cb_cmp_int, &search) == hp_at(&h, 2));
	search = 5;
	assert(hp_find(&h, cb_cmp_int, &search) == hp_at(&h, 4));
	search = 42;
	assert(hp_find(&h, cb_cmp_int, &search) == NULL);
	assert(hp_find(NULL, cb_cmp_int, &search) == NULL);
	assert(hp_find(&h, NULL, &search) == NULL);
	hp_delete(&h);
}

NO_COV void test_cm_hp_find_get(void) {
	t_cm_hp h;
	int     search;
	int     out;

	assert(hp_init(&h, 5, sizeof(int), cb_cmp_int) == &h);
	build_min(&h);

	search = 4;
	out    = 0;
	assert(hp_find_get(&h, cb_cmp_int, &search, &out) == true);
	assert(out == 4);

	search = 42;
	out    = 9;
	assert(hp_find_get(&h, cb_cmp_int, &search, &out) == false);
	assert(out == 9);

	assert(hp_find_get(NULL, cb_cmp_int, &search, &out) == false);
	assert(hp_find_get(&h, NULL, &search, &out) == false);
	assert(hp_find_get(&h, cb_cmp_int, &search, NULL) == false);
	hp_delete(&h);
}

NO_COV void test_cm_hp_fold(void) {
	t_cm_hp h;
	int     sum;

	assert(hp_init(&h, 5, sizeof(int), cb_cmp_int) == &h);
	build_min(&h);

	sum = 0;
	assert(hp_fold(&h, cb_merger, &sum) == &sum);
	assert(sum == 15);

	hp_clear(&h);
	sum = 42;
	assert(hp_fold(&h, cb_merger, &sum) == &sum);
	assert(sum == 42);

	assert(hp_fold(NULL, cb_merger, &sum) == NULL);
	assert(hp_fold(&h, NULL, &sum) == NULL);
	assert(hp_fold(&h, cb_merger, NULL) == NULL);
	hp_delete(&h);
}

NO_COV void test_cm_hp_reduce(void) {
	t_cm_hp h;
	int    *res;

	assert(hp_init(&h, 5, sizeof(int), cb_cmp_int) == &h);
	build_min(&h);

	alloc_set_call_limit(0);
	assert(hp_reduce(&h, cb_merger) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	res = hp_reduce(&h, cb_merger);
	assert(res != NULL);
	assert(*res == 15);
	free(res);

	hp_clear(&h);
	res = hp_reduce(&h, cb_merger);
	assert(res != NULL);
	assert(*res == 0);
	free(res);

	assert(hp_reduce(NULL, cb_merger) == NULL);
	assert(hp_reduce(&h, NULL) == NULL);
	hp_delete(&h);
}
