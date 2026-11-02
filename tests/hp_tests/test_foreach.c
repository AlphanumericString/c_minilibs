#include <assert.h>

#include "hp_tests.h"

NO_COV static void build_max(t_hp *h, int *vals) {
	for (size_t i = 0; i < 5; i++)
		assert(hp_max_push(h, &vals[i]) == true);
}

NO_COV void test_hp_foreach(void) {
	t_order_ctx ctx = {0};
	t_hp        h;
	int         in[]       = {0x40, 0x41, 0x42, 0x43, 0x44};
	int         expected[] = {0x44, 0x43, 0x41, 0x40, 0x42};

	hp_init(&h, 5, sizeof(int), cb_cmp_int);
	build_max(&h, in);

	assert(hp_foreach(&h, cb_log_int, &ctx) == true);
	for (size_t i = 0; i < 5; i++)
		assert(ctx.vals[i] == expected[i]);
	assert(ctx.n == 5);
	assert(hp_foreach(NULL, cb_log_int, &ctx) == false);
	assert(hp_foreach(&h, NULL, &ctx) == false);
	for (size_t i = 0; i < 5; i++)
		assert(hp_foreach(&h, cb_fail_at, &in[i]) == false);
	assert(hp_foreach(&h, cb_true, NULL) == true);
	assert(hp_foreach(&h, cb_false, NULL) == false);

	hp_delete(&h);
}
NO_COV void test_hp_foreach_in(void) {
	t_order_ctx ctx = {0};
	t_hp        h;
	int         in[]       = {0x40, 0x41, 0x42, 0x43, 0x44};
	int         expected[] = {0x40, 0x43, 0x42, 0x44, 0x41};

	hp_init(&h, 5, sizeof(int), cb_cmp_int);
	build_max(&h, in);

	assert(hp_foreach_in(&h, cb_log_int, &ctx) == true);
	for (size_t i = 0; i < 5; i++)
		assert(ctx.vals[i] == expected[i]);
	assert(ctx.n == 5);
	assert(hp_foreach_in(NULL, cb_log_int, &ctx) == false);
	assert(hp_foreach_in(&h, NULL, &ctx) == false);
	for (size_t i = 0; i < 5; i++)
		assert(hp_foreach_in(&h, cb_fail_at, &in[i]) == false);
	assert(hp_foreach_in(&h, cb_true, NULL) == true);
	assert(hp_foreach_in(&h, cb_false, NULL) == false);

	hp_delete(&h);
}
NO_COV void test_hp_foreach_pre(void) {
	t_order_ctx ctx = {0};
	t_hp        h;
	int         in[]       = {0x40, 0x41, 0x42, 0x43, 0x44};
	int         expected[] = {0x40, 0x42, 0x43, 0x41, 0x44};

	hp_init(&h, 5, sizeof(int), cb_cmp_int);
	build_max(&h, in);

	assert(hp_foreach_pre(&h, cb_log_int, &ctx) == true);
	for (size_t i = 0; i < 5; i++)
		assert(ctx.vals[i] == expected[i]);
	assert(ctx.n == 5);
	assert(hp_foreach_pre(NULL, cb_log_int, &ctx) == false);
	assert(hp_foreach_pre(&h, NULL, &ctx) == false);
	for (size_t i = 0; i < 5; i++)
		assert(hp_foreach_pre(&h, cb_fail_at, &in[i]) == false);
	assert(hp_foreach_pre(&h, cb_true, NULL) == true);
	assert(hp_foreach_pre(&h, cb_false, NULL) == false);

	hp_delete(&h);
}
NO_COV void test_hp_foreach_post(void) {
	t_order_ctx ctx = {0};
	t_hp        h;
	int         in[]       = {0x40, 0x41, 0x42, 0x43, 0x44};
	int         expected[] = {0x44, 0x43, 0x40, 0x42, 0x41};

	hp_init(&h, 5, sizeof(int), cb_cmp_int);
	build_max(&h, in);

	assert(hp_foreach_post(&h, cb_log_int, &ctx) == true);
	for (size_t i = 0; i < 5; i++)
		assert(ctx.vals[i] == expected[i]);
	assert(ctx.n == 5);
	assert(hp_foreach_post(NULL, cb_log_int, &ctx) == false);
	assert(hp_foreach_post(&h, NULL, &ctx) == false);
	for (size_t i = 0; i < 5; i++)
		assert(hp_foreach_post(&h, cb_fail_at, &in[i]) == false);
	assert(hp_foreach_post(&h, cb_true, NULL) == true);
	assert(hp_foreach_post(&h, cb_false, NULL) == false);

	hp_delete(&h);
}
