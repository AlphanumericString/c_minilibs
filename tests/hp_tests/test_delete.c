#include <assert.h>

#include "hp_tests.h"

static int cmp_rawptr(const void *a, const void *b) {
	if ((size_t)a < (size_t)b) return (-1);
	else if ((size_t)a > (size_t)b) return (1);
	return (0);
}

// delete
void test_hp_clear(void) {
	t_heap hp;

	hp_init(&hp, 32, &cmp_rawptr);
	assert(hp.elems != NULL);
	hp_max_push(&hp, (void *)0xDEAD);
	assert(hp.nb != 0);
	assert(hp.elems[0] != NULL);
	hp_clear(&hp);
	assert(hp.capacity == 32);
	assert(hp.nb == 0);
	assert(hp.elems != NULL);
	assert(hp.elems[0] != NULL);
	hp_delete(&hp);
}
void test_hp_delete(void) {
	t_heap hp;

	hp_init(&hp, 32, &cmp_rawptr);
	assert(hp.elems != NULL);
	hp_max_push(&hp, (void *)0xDEAD);
	assert(hp.nb != 0);
	hp_delete(&hp);
	assert(hp.elems == NULL);
	assert(hp.capacity == 0);
	assert(hp.nb == 0);
}
void test_hp_free(void) {
	t_heap *hp;

	hp = hp_create(32, &cmp_rawptr);
	assert(hp);
	assert(hp->elems != NULL);
	hp_max_push(hp, (void *)0xDEAD);
	assert(hp->nb != 0);
	hp_free(hp);
}
