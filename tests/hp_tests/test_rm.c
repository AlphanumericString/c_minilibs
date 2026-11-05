#include <assert.h>

#include "hp_tests.h"

NO_COV void test_cm_hp_max_rm(void) {
	t_cm_hp hp;
	int     in[]    = {0x40, 0x41, 0x42, 0x39, 0x37, 0x36, 0x35};
	int     order[] = {0x42, 0x41, 0x40, 0x39, 0x37, 0x36, 0x35};
	int     out;

	assert(hp_init(&hp, 32, sizeof(int), cb_cmp_int) == &hp);
	assert(hp_max_push(&hp, &in[0]) == true);
	assert(hp_max_push(&hp, &in[1]) == true);
	assert(hp_max_push(&hp, &in[2]) == true);
	assert(hp_max_push(&hp, &in[3]) == true);
	assert(hp.nb == 4);
	assert(((int *)hp.elems)[0] == 0x42);
	assert(((int *)hp.elems)[1] == 0x40);
	assert(((int *)hp.elems)[2] == 0x41);
	assert(((int *)hp.elems)[3] == 0x39);

	assert(hp_max_rm(&hp, 1, &out) == true);
	assert(out == 0x40);

	assert(hp.nb == 3);
	assert(((int *)hp.elems)[0] == 0x42);
	assert(((int *)hp.elems)[1] == 0x39);
	assert(((int *)hp.elems)[2] == 0x41);

	assert(hp_max_rm(&hp, (size_t)-1, &out) == true);
	assert(out == 0x41);
	assert(hp.nb == 2);
	assert(((int *)hp.elems)[0] == 0x42);
	assert(((int *)hp.elems)[1] == 0x39);

	assert(hp_max_rm(&hp, 0, &out) == true);
	assert(out == 0x42);
	assert(hp.nb == 1);
	assert(((int *)hp.elems)[0] == 0x39);

	assert(hp_min_rm(&hp, 0, &out) == true);
	assert(out == 0x39);
	assert(hp.nb == 0);

	assert(hp_max_rm(NULL, 1, &out) == false);
	assert(hp_max_rm(&hp, 1, &out) == false);
	assert(hp_max_rm(&hp, 1, NULL) == false);

	hp_clear(&hp);
	for (size_t i = 0; i < 7; i++)
		assert(hp_max_push(&hp, &in[i]) == true);
	for (size_t i = 0; hp.nb; i++) {
		assert(hp_max_pop(&hp, &out) == true);
		assert(out == order[i]);
	}

	hp_clear(&hp);
	assert(hp_max_push(&hp, &in[5]) == true);
	assert(hp_max_push(&hp, &in[2]) == true);
	assert(hp_max_push(&hp, &in[3]) == true);
	assert(hp_max_push(&hp, &in[0]) == true);
	assert(hp_max_push(&hp, &in[1]) == true);
	assert(hp_max_push(&hp, &in[4]) == true);
	assert(hp_max_push(&hp, &in[6]) == true);
	for (size_t i = 0; hp.nb; i++) {
		assert(hp_max_pop(&hp, &out) == true);
		assert(out == order[i]);
	}

	hp_delete(&hp);
	return;
}
NO_COV void test_cm_hp_min_rm(void) {
	t_cm_hp hp;
	int     in[]    = {0x40, 0x41, 0x42, 0x39, 0x37, 0x36, 0x35};
	int     order[] = {0x35, 0x36, 0x37, 0x39, 0x40, 0x41, 0x42};
	int     out;

	assert(hp_init(&hp, 32, sizeof(int), cb_cmp_int) == &hp);
	assert(hp_min_push(&hp, &in[0]) == true);
	assert(hp_min_push(&hp, &in[1]) == true);
	assert(hp_min_push(&hp, &in[2]) == true);
	assert(hp_min_push(&hp, &in[3]) == true);
	assert(hp.nb == 4);
	assert(((int *)hp.elems)[0] == 0x39);
	assert(((int *)hp.elems)[1] == 0x40);
	assert(((int *)hp.elems)[2] == 0x42);
	assert(((int *)hp.elems)[3] == 0x41);

	assert(hp_min_rm(&hp, 1, &out) == true);
	assert(out == 0x40);
	assert(hp.nb == 3);
	assert(((int *)hp.elems)[0] == 0x39);
	assert(((int *)hp.elems)[1] == 0x41);
	assert(((int *)hp.elems)[2] == 0x42);

	assert(hp_min_rm(&hp, (size_t)-1, &out) == true);
	assert(out == 0x42);
	assert(hp.nb == 2);
	assert(((int *)hp.elems)[0] == 0x39);
	assert(((int *)hp.elems)[1] == 0x41);

	assert(hp_min_rm(&hp, 0, &out) == true);
	assert(out == 0x39);
	assert(hp.nb == 1);
	assert(((int *)hp.elems)[0] == 0x41);

	assert(hp_min_rm(&hp, 0, &out) == true);
	assert(out == 0x41);
	assert(hp.nb == 0);

	assert(hp_min_rm(NULL, 1, &out) == false);
	assert(hp_min_rm(&hp, 1, &out) == false);
	assert(hp_min_rm(&hp, 1, NULL) == false);

	hp_clear(&hp);
	for (size_t i = 0; i < 7; i++)
		assert(hp_min_push(&hp, &in[i]) == true);
	for (size_t i = 0; hp.nb; i++) {
		assert(hp_min_pop(&hp, &out) == true);
		assert(out == order[i]);
	}

	hp_clear(&hp);
	assert(hp_min_push(&hp, &in[5]) == true);
	assert(hp_min_push(&hp, &in[2]) == true);
	assert(hp_min_push(&hp, &in[3]) == true);
	assert(hp_min_push(&hp, &in[0]) == true);
	assert(hp_min_push(&hp, &in[1]) == true);
	assert(hp_min_push(&hp, &in[4]) == true);
	assert(hp_min_push(&hp, &in[6]) == true);
	for (size_t i = 0; hp.nb; i++) {
		assert(hp_min_pop(&hp, &out) == true);
		assert(out == order[i]);
	}

	hp_delete(&hp);
	return;
}
