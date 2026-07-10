#include <assert.h>
#include <hp/heap.h>

#include "hp_tests.h"

static int cmp_rawptr(const void *a, const void *b) {
	if ((size_t)a < (size_t)b) return (-1);
	else if ((size_t)a > (size_t)b) return (1);
	return (0);
}

void test_hp_max_rm(void) {
	t_heap hp;

	hp_init(&hp, 32, cmp_rawptr);
	hp_max_push(&hp, (void *)0x40);
	hp_max_push(&hp, (void *)0x41);
	hp_max_push(&hp, (void *)0x42);
	hp_max_push(&hp, (void *)0x39);
	assert(hp.nb == 4);
	assert(hp.elems[0] == (void *)0x42);
	assert(hp.elems[1] == (void *)0x40);
	assert(hp.elems[2] == (void *)0x41);
	assert(hp.elems[3] == (void *)0x39);

	assert(hp_max_rm(&hp, 1) == (void *)0x40);
	assert(hp.nb == 3);
	assert(hp.elems[0] == (void *)0x42);
	assert(hp.elems[1] == (void *)0x39);
	assert(hp.elems[2] == (void *)0x41);

	hp_delete(&hp);
	return;
}
void test_hp_min_rm(void) {
	t_heap hp;

	hp_init(&hp, 32, cmp_rawptr);
	hp_min_push(&hp, (void *)0x40);
	hp_min_push(&hp, (void *)0x41);
	hp_min_push(&hp, (void *)0x42);
	hp_min_push(&hp, (void *)0x39);
	assert(hp.nb == 4);
	assert(hp.elems[0] == (void *)0x39);
	assert(hp.elems[1] == (void *)0x40);
	assert(hp.elems[2] == (void *)0x42);
	assert(hp.elems[3] == (void *)0x41);

	assert(hp_min_rm(&hp, 1) == (void *)0x40);
	assert(hp.nb == 3);
	assert(hp.elems[0] == (void *)0x39);
	assert(hp.elems[1] == (void *)0x41);
	assert(hp.elems[2] == (void *)0x42);

	hp_delete(&hp);
	return;
}
