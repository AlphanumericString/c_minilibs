#include <assert.h>
#include <hp/heap.h>

#include "hp_tests.h"

static int cmp_rawptr(const void *a, const void *b) {
	if ((size_t)a < (size_t)b) return (-1);
	else if ((size_t)a > (size_t)b) return (1);
	return (0);
}

NO_COV void test_hp_max_rm(void) {
	t_hp  hp;
	void *indexes[] = {
	    (void *)0x42, (void *)0x41, (void *)0x40, (void *)0x39,
	    (void *)0x37, (void *)0x36, (void *)0x35,
	};

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

	assert(hp_max_rm(&hp, -1) == (void *)0x41);
	assert(hp.nb == 2);
	assert(hp.elems[0] == (void *)0x42);
	assert(hp.elems[1] == (void *)0x39);

	assert(hp_max_rm(&hp, 0) == (void *)0x42);
	assert(hp.nb == 1);
	assert(hp.elems[0] == (void *)0x39);

	assert(hp_min_rm(&hp, 0) == (void *)0x39);
	assert(hp.nb == 0);

	assert(hp_max_rm(NULL, 1) == NULL);

	hp_clear(&hp);
	hp_max_push(&hp, (void *)0x42);
	hp_max_push(&hp, (void *)0x40);
	hp_max_push(&hp, (void *)0x41);
	hp_max_push(&hp, (void *)0x39);
	hp_max_push(&hp, (void *)0x37);
	hp_max_push(&hp, (void *)0x36);
	hp_max_push(&hp, (void *)0x35);
	for (size_t i = 0; hp.nb; i++)
		assert(hp_max_pop(&hp) == indexes[i]);

	hp_clear(&hp);
	hp_max_push(&hp, (void *)0x36);
	hp_max_push(&hp, (void *)0x42);
	hp_max_push(&hp, (void *)0x39);
	hp_max_push(&hp, (void *)0x40);
	hp_max_push(&hp, (void *)0x41);
	hp_max_push(&hp, (void *)0x37);
	hp_max_push(&hp, (void *)0x35);
	for (size_t i = 0; hp.nb; i++)
		assert(hp_max_pop(&hp) == indexes[i]);

	hp_delete(&hp);
	return;
}
NO_COV void test_hp_min_rm(void) {
	t_hp  hp;
	void *indexes[] = {(void *)0x35, (void *)0x36, (void *)0x37, (void *)0x39,
	                   (void *)0x40, (void *)0x41, (void *)0x42};

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

	assert(hp_min_rm(&hp, -1) == (void *)0x42);
	assert(hp.nb == 2);
	assert(hp.elems[0] == (void *)0x39);
	assert(hp.elems[1] == (void *)0x41);

	assert(hp_min_rm(&hp, 0) == (void *)0x39);
	assert(hp.nb == 1);
	assert(hp.elems[0] == (void *)0x41);

	assert(hp_min_rm(&hp, 0) == (void *)0x41);
	assert(hp.nb == 0);

	assert(hp_min_rm(NULL, 1) == NULL);

	hp_clear(&hp);
	hp_min_push(&hp, (void *)0x42);
	hp_min_push(&hp, (void *)0x40);
	hp_min_push(&hp, (void *)0x41);
	hp_min_push(&hp, (void *)0x39);
	hp_min_push(&hp, (void *)0x37);
	hp_min_push(&hp, (void *)0x36);
	hp_min_push(&hp, (void *)0x35);
	for (size_t i = 0; hp.nb; i++)
		assert(hp_min_pop(&hp) == indexes[i]);

	hp_clear(&hp);
	hp_min_push(&hp, (void *)0x36);
	hp_min_push(&hp, (void *)0x42);
	hp_min_push(&hp, (void *)0x39);
	hp_min_push(&hp, (void *)0x40);
	hp_min_push(&hp, (void *)0x41);
	hp_min_push(&hp, (void *)0x37);
	hp_min_push(&hp, (void *)0x35);
	for (size_t i = 0; hp.nb; i++)
		assert(hp_min_pop(&hp) == indexes[i]);

	hp_delete(&hp);
	return;
}
