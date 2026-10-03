#include <assert.h>

#include "hp_tests.h"

static int cmp_rawptr(const void *a, const void *b) {
	if ((size_t)a < (size_t)b) return (-1);
	else if ((size_t)a > (size_t)b) return (1);
	return (0);
}

void test_hp_min_push(void) {
	t_heap hp;

	hp_init(&hp, 32, &cmp_rawptr);
	assert(hp.elems != NULL);
	hp_min_push(&hp, (void *)0x42);
	assert(hp.nb == 1);
	assert(hp.elems[0] == (void *)0x42);
	hp_min_push(&hp, (void *)0x40);
	assert(hp.nb == 2);
	assert(hp.elems[0] == (void *)0x40);
	assert(hp.elems[1] == (void *)0x42);
	hp_min_push(&hp, (void *)0x41);
	assert(hp.nb == 3);
	assert(hp.elems[0] == (void *)0x40);
	assert(hp.elems[1] == (void *)0x42);
	assert(hp.elems[2] == (void *)0x41);
	hp_min_push(&hp, (void *)0x39);
	assert(hp.nb == 4);
	assert(hp.elems[0] == (void *)0x39);
	assert(hp.elems[1] == (void *)0x40);
	assert(hp.elems[2] == (void *)0x41);
	assert(hp.elems[3] == (void *)0x42);
	hp_min_push(NULL, (void *)0x39);
	assert(hp.nb == 4);
	hp_min_push(&hp, NULL);
	assert(hp.nb == 4);
	hp.capacity = hp.nb;
	hp_min_push(&hp, (void *)0x43);
	assert(hp.nb == 4);

	hp_delete(&hp);
	return;
}
void test_hp_max_push(void) {
	t_heap hp;

	hp_init(&hp, 32, &cmp_rawptr);
	assert(hp.elems != NULL);
	hp_max_push(&hp, (void *)0x39);
	assert(hp.nb == 1);
	assert(hp.elems[0] == (void *)0x39);
	hp_max_push(&hp, (void *)0x40);
	assert(hp.nb == 2);
	assert(hp.elems[0] == (void *)0x40);
	assert(hp.elems[1] == (void *)0x39);
	hp_max_push(&hp, (void *)0x38);
	assert(hp.nb == 3);
	assert(hp.elems[0] == (void *)0x40);
	assert(hp.elems[1] == (void *)0x39);
	assert(hp.elems[2] == (void *)0x38);
	hp_max_push(&hp, (void *)0x42);
	assert(hp.nb == 4);
	assert(hp.elems[0] == (void *)0x42);
	assert(hp.elems[1] == (void *)0x40);
	assert(hp.elems[2] == (void *)0x38);
	assert(hp.elems[3] == (void *)0x39);
	hp_max_push(NULL, (void *)0x39);
	assert(hp.nb == 4);
	hp_max_push(&hp, NULL);
	assert(hp.nb == 4);
	hp.capacity = hp.nb;
	hp_max_push(&hp, (void *)0x43);
	assert(hp.nb == 4);

	hp_delete(&hp);
	return;
}
