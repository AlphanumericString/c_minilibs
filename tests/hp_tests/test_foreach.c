#include <assert.h>

#include "hp_tests.h"

static int cmp_rawptr(const void *a, const void *b) {
	if ((size_t)a < (size_t)b) return (-1);
	else if ((size_t)a > (size_t)b) return (1);
	return (0);
}

// TODO: add fail test at 0-5 sweeping through values to test random error in
// process for base but also 'pre/post/in'
static bool fail_at(void *hp_elem, void *value) {
	return (hp_elem != value);
}

static bool log_order(void *hp_elem, void *_order) {
	void **order = (void **)_order;
	size_t i;

	for (i = 0; order[i] != NULL; i++)
		;
	order[i] = hp_elem;
	return (true);
}

void test_hp_foreach(void) {
	void  *order[6] = {0};
	t_heap h;

	hp_init(&h, 5, cmp_rawptr);
	hp_max_push(&h, (void *)0x40);
	hp_max_push(&h, (void *)0x41);
	hp_max_push(&h, (void *)0x42);
	hp_max_push(&h, (void *)0x43);
	hp_max_push(&h, (void *)0x44);

	assert(hp_foreach(&h, log_order, &order) == true);
	assert(order[0] == (void *)0x44);
	assert(order[1] == (void *)0x43);
	assert(order[2] == (void *)0x41);
	assert(order[3] == (void *)0x40);
	assert(order[4] == (void *)0x42);
	assert(order[5] == NULL);
	assert(hp_foreach(NULL, log_order, &order) == false);
	assert(hp_foreach(&h, NULL, &order) == false);
	assert(hp_foreach(&h, fail_at, (void *)0x40L) == false);
	assert(hp_foreach(&h, fail_at, (void *)0x41L) == false);
	assert(hp_foreach(&h, fail_at, (void *)0x42L) == false);
	assert(hp_foreach(&h, fail_at, (void *)0x43L) == false);
	assert(hp_foreach(&h, fail_at, (void *)0x44L) == false);

	hp_delete(&h);
}
void test_hp_foreach_in(void) {
	void  *order[6] = {0};
	t_heap h;

	hp_init(&h, 5, cmp_rawptr);
	hp_max_push(&h, (void *)0x40);
	hp_max_push(&h, (void *)0x41);
	hp_max_push(&h, (void *)0x42);
	hp_max_push(&h, (void *)0x43);
	hp_max_push(&h, (void *)0x44);

	assert(hp_foreach_in(&h, log_order, &order) == true);
	assert(order[0] == (void *)0x40);
	assert(order[1] == (void *)0x43);
	assert(order[2] == (void *)0x42);
	assert(order[3] == (void *)0x44);
	assert(order[4] == (void *)0x41);
	assert(order[5] == NULL);
	assert(hp_foreach_in(NULL, log_order, &order) == false);
	assert(hp_foreach_in(&h, NULL, &order) == false);
	assert(hp_foreach_in(&h, fail_at, (void *)0x40L) == false);
	assert(hp_foreach_in(&h, fail_at, (void *)0x41L) == false);
	assert(hp_foreach_in(&h, fail_at, (void *)0x42L) == false);
	assert(hp_foreach_in(&h, fail_at, (void *)0x43L) == false);
	assert(hp_foreach_in(&h, fail_at, (void *)0x44L) == false);

	hp_delete(&h);
}
void test_hp_foreach_pre(void) {
	void  *order[6] = {0};
	t_heap h;

	hp_init(&h, 5, cmp_rawptr);
	hp_max_push(&h, (void *)0x40);
	hp_max_push(&h, (void *)0x41);
	hp_max_push(&h, (void *)0x42);
	hp_max_push(&h, (void *)0x43);
	hp_max_push(&h, (void *)0x44);

	assert(hp_foreach_pre(&h, log_order, &order) == true);
	assert(order[0] == (void *)0x40);
	assert(order[1] == (void *)0x42);
	assert(order[2] == (void *)0x43);
	assert(order[3] == (void *)0x41);
	assert(order[4] == (void *)0x44);
	assert(order[5] == NULL);
	assert(hp_foreach_pre(NULL, log_order, &order) == false);
	assert(hp_foreach_pre(&h, NULL, &order) == false);
	assert(hp_foreach_pre(&h, fail_at, (void *)0x40L) == false);
	assert(hp_foreach_pre(&h, fail_at, (void *)0x41L) == false);
	assert(hp_foreach_pre(&h, fail_at, (void *)0x42L) == false);
	assert(hp_foreach_pre(&h, fail_at, (void *)0x43L) == false);
	assert(hp_foreach_pre(&h, fail_at, (void *)0x44L) == false);

	hp_delete(&h);
}
void test_hp_foreach_post(void) {
	void  *order[6] = {0};
	t_heap h;

	hp_init(&h, 5, cmp_rawptr);
	hp_max_push(&h, (void *)0x40);
	hp_max_push(&h, (void *)0x41);
	hp_max_push(&h, (void *)0x42);
	hp_max_push(&h, (void *)0x43);
	hp_max_push(&h, (void *)0x44);

	assert(hp_foreach_post(&h, log_order, &order) == true);
	assert(order[0] == (void *)0x44);
	assert(order[1] == (void *)0x43);
	assert(order[2] == (void *)0x40);
	assert(order[3] == (void *)0x42);
	assert(order[4] == (void *)0x41);
	assert(order[5] == NULL);
	assert(hp_foreach_post(NULL, log_order, &order) == false);
	assert(hp_foreach_post(&h, NULL, &order) == false);
	assert(hp_foreach_post(&h, fail_at, (void *)0x40L) == false);
	assert(hp_foreach_post(&h, fail_at, (void *)0x41L) == false);
	assert(hp_foreach_post(&h, fail_at, (void *)0x42L) == false);
	assert(hp_foreach_post(&h, fail_at, (void *)0x43L) == false);
	assert(hp_foreach_post(&h, fail_at, (void *)0x44L) == false);

	hp_delete(&h);
}
