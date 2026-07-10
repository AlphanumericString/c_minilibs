#include <assert.h>
#include <stddef.h>

#include "hp_tests.h"

static int cmp_rawptr(const void *a, const void *b) {
	if ((size_t)a < (size_t)b) return (-1);
	else if ((size_t)a > (size_t)b) return (1);
	return (0);
}

// create
void test_hp_create(void) {
	t_heap *h;

	h = hp_create(32, cmp_rawptr);
	assert(h != NULL);
	assert(h->elems != NULL);
	assert(h->nb == 0);
	assert(h->capacity == 32);
	hp_free(h);
}
void test_hp_init(void) {
	t_heap h;

	assert(hp_init(&h, 32, cmp_rawptr) == &h);
	assert(h.elems != NULL);
	assert(h.capacity == 32);
	assert(h.nb == 0);
	hp_delete(&h);
}
