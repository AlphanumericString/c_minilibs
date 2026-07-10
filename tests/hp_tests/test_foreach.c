#include <assert.h>

#include "hp_tests.h"

static int cmp_rawptr(const void *a, const void *b) {
	if ((size_t)a < (size_t)b) return (-1);
	else if ((size_t)a > (size_t)b) return (1);
	return (0);
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

	hp_foreach(&h, log_order, &order);
	assert(order[0] == (void *)0x44);
	assert(order[1] == (void *)0x43);
	assert(order[2] == (void *)0x41);
	assert(order[3] == (void *)0x40);
	assert(order[4] == (void *)0x42);
	assert(order[5] == NULL);
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

	hp_foreach_in(&h, log_order, &order);
	assert(order[0] == (void *)0x40);
	assert(order[1] == (void *)0x43);
	assert(order[2] == (void *)0x42);
	assert(order[3] == (void *)0x44);
	assert(order[4] == (void *)0x41);
	assert(order[5] == NULL);
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

	hp_foreach_pre(&h, log_order, &order);
	assert(order[0] == (void *)0x40);
	assert(order[1] == (void *)0x42);
	assert(order[2] == (void *)0x43);
	assert(order[3] == (void *)0x41);
	assert(order[4] == (void *)0x44);
	assert(order[5] == NULL);

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

	hp_foreach_post(&h, log_order, &order);
	assert(order[0] == (void *)0x44);
	assert(order[1] == (void *)0x43);
	assert(order[2] == (void *)0x40);
	assert(order[3] == (void *)0x42);
	assert(order[4] == (void *)0x41);
	assert(order[5] == NULL);

	hp_delete(&h);
}
