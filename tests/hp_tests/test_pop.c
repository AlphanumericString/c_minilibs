#include <assert.h>

#include "hp_tests.h"

NO_COV void test_cm_hp_min_pop(void) {
	t_cm_hp hp;
	int     v40 = 0x40;
	int     v41 = 0x41;
	int     v42 = 0x42;
	int     out = 0x1234;

	assert(hp_init(&hp, 32, sizeof(int), cb_cmp_int) == &hp);
	assert(hp.elems != NULL);
	assert(hp_min_push(&hp, &v42) == true);
	assert(hp_min_push(&hp, &v40) == true);
	assert(hp_min_push(&hp, &v41) == true);
	assert(hp.nb == 3);
	assert(hp_min_pop(&hp, &out) == true);
	assert(out == 0x40);
	assert(hp_min_pop(&hp, &out) == true);
	assert(out == 0x41);
	assert(hp_min_pop(&hp, &out) == true);
	assert(out == 0x42);
	out = 0x1234;
	assert(hp_min_pop(&hp, &out) == false);
	assert(out == 0x1234);
	assert(hp_min_pop(NULL, &out) == false);
	assert(hp_min_pop(&hp, NULL) == false);
	hp.nb   = 1;
	hp.sz_e = 0;
	out     = 0x1234;
	assert(hp_min_pop(&hp, &out) == false);
	assert(out == 0x1234);
	hp_delete(&hp);
	return;
}
NO_COV void test_cm_hp_max_pop(void) {
	t_cm_hp hp;
	int     v40 = 0x40;
	int     v41 = 0x41;
	int     v42 = 0x42;
	int     out = 0x1234;

	assert(hp_init(&hp, 32, sizeof(int), cb_cmp_int) == &hp);
	assert(hp.elems != NULL);
	assert(hp_max_push(&hp, &v42) == true);
	assert(hp_max_push(&hp, &v40) == true);
	assert(hp_max_push(&hp, &v41) == true);
	assert(hp_max_pop(&hp, &out) == true);
	assert(out == 0x42);
	assert(hp_max_pop(&hp, &out) == true);
	assert(out == 0x41);
	assert(hp_max_pop(&hp, &out) == true);
	assert(out == 0x40);
	out = 0x1234;
	assert(hp_max_pop(&hp, &out) == false);
	assert(out == 0x1234);
	assert(hp_max_pop(NULL, &out) == false);
	assert(hp_max_pop(&hp, NULL) == false);
	hp.nb   = 1;
	hp.sz_e = 0;
	out     = 0x1234;
	assert(hp_max_pop(&hp, &out) == false);
	assert(out == 0x1234);
	hp_delete(&hp);
	return;
}
