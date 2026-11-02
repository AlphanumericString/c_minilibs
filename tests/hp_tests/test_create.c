#include <assert.h>

#include "hp_tests.h"

NO_COV void test_hp_create(void) {
	t_hp *h;

	h = hp_create(32, sizeof(int), cb_cmp_int);
	assert(h != NULL);
	assert(h->elems != NULL);
	assert(h->nb == 0);
	assert(h->capacity == 32);
	assert(h->sz_e == sizeof(int));
	hp_free(&h);

	alloc_set_call_limit(0);
	assert(hp_create(32, sizeof(int), cb_cmp_int) == NULL);
	alloc_reset_counter();
	alloc_set_call_limit(1);
	assert(hp_create(32, sizeof(int), cb_cmp_int) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	assert(hp_create(32, 0, cb_cmp_int) == NULL);
}
NO_COV void test_hp_init(void) {
	t_hp h;

	assert(hp_init(&h, 32, sizeof(int), cb_cmp_int) == &h);
	assert(h.elems != NULL);
	assert(h.capacity == 32);
	assert(h.sz_e == sizeof(int));
	assert(h.nb == 0);
	hp_delete(&h);

	alloc_set_call_limit(0);
	assert(hp_init(&h, 32, sizeof(int), cb_cmp_int) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	assert(hp_init(NULL, 32, sizeof(int), cb_cmp_int) == NULL);
	assert(hp_init(&h, 32, 0, cb_cmp_int) == NULL);
}
