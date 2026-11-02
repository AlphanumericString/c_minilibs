#include <assert.h>

#include "hp_tests.h"

NO_COV void test_hp_min_push(void) {
	t_hp hp;
	int  v39 = 0x39;
	int  v40 = 0x40;
	int  v41 = 0x41;
	int  v42 = 0x42;
	int  v43 = 0x43;

	assert(hp_init(&hp, 32, sizeof(int), cb_cmp_int) == &hp);
	assert(hp.elems != NULL);
	assert(hp_min_push(&hp, &v42) == true);
	assert(hp.nb == 1);
	assert(((int *)hp.elems)[0] == 0x42);
	assert(hp_min_push(&hp, &v40) == true);
	assert(hp.nb == 2);
	assert(((int *)hp.elems)[0] == 0x40);
	assert(((int *)hp.elems)[1] == 0x42);
	assert(hp_min_push(&hp, &v41) == true);
	assert(hp.nb == 3);
	assert(((int *)hp.elems)[0] == 0x40);
	assert(((int *)hp.elems)[1] == 0x42);
	assert(((int *)hp.elems)[2] == 0x41);
	assert(hp_min_push(&hp, &v39) == true);
	assert(hp.nb == 4);
	assert(((int *)hp.elems)[0] == 0x39);
	assert(((int *)hp.elems)[1] == 0x40);
	assert(((int *)hp.elems)[2] == 0x41);
	assert(((int *)hp.elems)[3] == 0x42);
	assert(hp_min_push(NULL, &v39) == false);
	assert(hp.nb == 4);
	assert(hp_min_push(&hp, NULL) == false);
	assert(hp.nb == 4);
	hp.capacity = hp.nb;
	assert(hp_min_push(&hp, &v43) == false);
	assert(hp.nb == 4);
	hp.capacity = 32;
	hp.sz_e     = 0;
	assert(hp_min_push(&hp, &v43) == false);
	assert(hp.nb == 4);

	hp_delete(&hp);
	return;
}
NO_COV void test_hp_max_push(void) {
	t_hp hp;
	int  v38 = 0x38;
	int  v39 = 0x39;
	int  v40 = 0x40;
	int  v42 = 0x42;
	int  v43 = 0x43;

	assert(hp_init(&hp, 32, sizeof(int), cb_cmp_int) == &hp);
	assert(hp.elems != NULL);
	assert(hp_max_push(&hp, &v39) == true);
	assert(hp.nb == 1);
	assert(((int *)hp.elems)[0] == 0x39);
	assert(hp_max_push(&hp, &v40) == true);
	assert(hp.nb == 2);
	assert(((int *)hp.elems)[0] == 0x40);
	assert(((int *)hp.elems)[1] == 0x39);
	assert(hp_max_push(&hp, &v38) == true);
	assert(hp.nb == 3);
	assert(((int *)hp.elems)[0] == 0x40);
	assert(((int *)hp.elems)[1] == 0x39);
	assert(((int *)hp.elems)[2] == 0x38);
	assert(hp_max_push(&hp, &v42) == true);
	assert(hp.nb == 4);
	assert(((int *)hp.elems)[0] == 0x42);
	assert(((int *)hp.elems)[1] == 0x40);
	assert(((int *)hp.elems)[2] == 0x38);
	assert(((int *)hp.elems)[3] == 0x39);
	assert(hp_max_push(NULL, &v39) == false);
	assert(hp.nb == 4);
	assert(hp_max_push(&hp, NULL) == false);
	assert(hp.nb == 4);
	hp.capacity = hp.nb;
	assert(hp_max_push(&hp, &v43) == false);
	assert(hp.nb == 4);
	hp.capacity = 32;
	hp.sz_e     = 0;
	assert(hp_max_push(&hp, &v43) == false);
	assert(hp.nb == 4);

	hp_delete(&hp);
	return;
}
