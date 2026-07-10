#include <assert.h>
#include <stdio.h>

#include "hp_tests.h"

static int cmp_rawptr(const void *a, const void *b) {
	if ((size_t)a < (size_t)b) return (-1);
	else if ((size_t)a > (size_t)b) return (1);
	return (0);
}

void test_hp_min_pop(void) {
	t_heap hp;
	void  *r;

	hp_init(&hp, 32, &cmp_rawptr);
	assert(hp.elems != NULL);
	hp_min_push(&hp, (void *)0x42);
	hp_min_push(&hp, (void *)0x40);
	hp_min_push(&hp, (void *)0x41);
	assert(hp.nb > 0);
	assert(hp.nb == 3);
	r = hp_min_pop(&hp);
	assert(r == (void *)0x40);
	r = hp_min_pop(&hp);
	assert(r == (void *)0x41);
	r = hp_min_pop(&hp);
	assert(r == (void *)0x42);
	assert(hp_min_pop(&hp) == NULL);
	hp_delete(&hp);
	return;
}
void test_hp_max_pop(void) {
	t_heap hp;

	hp_init(&hp, 32, &cmp_rawptr);
	assert(hp.elems != NULL);
	hp_max_push(&hp, (void *)0x42);
	hp_max_push(&hp, (void *)0x40);
	hp_max_push(&hp, (void *)0x41);
	assert(hp_max_pop(&hp) == (void *)0x42);
	assert(hp_max_pop(&hp) == (void *)0x41);
	assert(hp_max_pop(&hp) == (void *)0x40);
	assert(hp_max_pop(&hp) == NULL);
	hp_delete(&hp);
	return;
}
