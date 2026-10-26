#include <assert.h>
#include <stdlib.h>

#include "hp_tests.h"

static int cmp_rawptr(const void *a, const void *b) {
	if ((size_t)a < (size_t)b) return (-1);
	else if ((size_t)a > (size_t)b) return (1);
	return (0);
}

NO_COV void test_hp_clear(void) {
	t_hp hp;

	hp_init(&hp, 32, &cmp_rawptr);
	assert(hp.elems != NULL);
	hp_max_push(&hp, (void *)0xDEAD);
	assert(hp.nb != 0);
	assert(hp.elems[0] != NULL);
	assert(hp_clear(&hp) == true);
	assert(hp_clear(&hp) == true);
	assert(hp_clear(&hp) == true);
	assert(hp.capacity == 32);
	assert(hp.nb == 0);
	assert(hp.elems != NULL);
	assert(hp.elems[0] != NULL);
	hp_delete(&hp);
	assert(hp_clear(NULL) == false);
}
NO_COV void test_hp_delete(void) {
	t_hp hp;

	hp_init(&hp, 32, &cmp_rawptr);
	assert(hp.elems != NULL);
	hp_max_push(&hp, (void *)0xDEAD);
	assert(hp.nb != 0);
	assert(hp_delete(&hp) == true);
	assert(hp.elems == NULL);
	assert(hp.capacity == 0);
	assert(hp.nb == 0);
	assert(hp_delete(NULL) == false);
	hp = (t_hp){.elems = NULL, .capacity = 12, .cmp = NULL, .nb = 6};
	assert(hp_delete(&hp) == true);
	assert(hp.capacity == 0 && hp.cmp == NULL && hp.nb == 0 &&
	       hp.elems == NULL);
}
NO_COV void test_hp_free(void) {
	t_hp *hp;
	void *no_leaks;

	hp = hp_create(32, &cmp_rawptr);
	assert(hp);
	assert(hp->elems != NULL);
	hp_max_push(hp, (void *)0xDEAD);
	assert(hp->nb != 0);
	assert(hp != NULL);
	assert(hp_free(&hp) == true);
	assert(hp == NULL);
	assert(hp_free(&hp) == true);
	assert(hp_free(NULL) == false);
	hp        = hp_create(32, &cmp_rawptr);
	no_leaks  = hp->elems;
	hp->elems = NULL;
	assert(hp_free(&hp) == true);
	free(no_leaks);
}
