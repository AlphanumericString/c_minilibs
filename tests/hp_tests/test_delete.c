#include <assert.h>
#include <stdlib.h>

#include "hp_tests.h"

NO_COV void test_hp_clear(void) {
	t_hp hp;
	int  v = 0xDEAD;

	hp_init(&hp, 32, sizeof(int), cb_cmp_int);
	assert(hp.elems != NULL);
	assert(hp_max_push(&hp, &v) == true);
	assert(hp.nb != 0);
	assert(hp_clear(&hp) == true);
	assert(hp_clear(&hp) == true);
	assert(hp_clear(&hp) == true);
	assert(hp.capacity == 32);
	assert(hp.nb == 0);
	assert(hp.elems != NULL);
	hp_delete(&hp);
	assert(hp_clear(NULL) == false);
}
NO_COV void test_hp_delete(void) {
	t_hp hp;
	int  v = 0xDEAD;

	hp_init(&hp, 32, sizeof(int), cb_cmp_int);
	assert(hp.elems != NULL);
	assert(hp_max_push(&hp, &v) == true);
	assert(hp.nb != 0);
	assert(hp_delete(&hp) == true);
	assert(hp.elems == NULL);
	assert(hp.capacity == 0);
	assert(hp.nb == 0);
	assert(hp.sz_e == 0);
	assert(hp_delete(NULL) == false);
	hp = (t_hp){.elems = NULL, .capacity = 12, .cmp = NULL, .nb = 6, .sz_e = 4};
	assert(hp_delete(&hp) == true);
	assert(hp.capacity == 0 && hp.cmp == NULL && hp.nb == 0 &&
	       hp.elems == NULL && hp.sz_e == 0);
}
NO_COV void test_hp_free(void) {
	t_hp *hp;
	void *no_leaks;
	int   v = 0xDEAD;

	hp = hp_create(32, sizeof(int), cb_cmp_int);
	assert(hp);
	assert(hp->elems != NULL);
	assert(hp_max_push(hp, &v) == true);
	assert(hp->nb != 0);
	assert(hp != NULL);
	assert(hp_free(&hp) == true);
	assert(hp == NULL);
	assert(hp_free(&hp) == true);
	assert(hp_free(NULL) == false);
	hp        = hp_create(32, sizeof(int), cb_cmp_int);
	no_leaks  = hp->elems;
	hp->elems = NULL;
	assert(hp_free(&hp) == true);
	free(no_leaks);
}
