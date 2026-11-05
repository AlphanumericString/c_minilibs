#include <assert.h>

#include "hp_tests.h"

NO_COV void test_cm_hp_get_cmp(void) {
	t_cm_hp hp;

	hp_init(&hp, 4, sizeof(int), cb_cmp_int);
	assert(hp_get_cmp(&hp) == cb_cmp_int);
	hp_delete(&hp);
	assert(hp_get_cmp(NULL) == NULL);
}
NO_COV void test_cm_hp_get_cap(void) {
	t_cm_hp hp;

	hp_init(&hp, 4, sizeof(int), cb_cmp_int);
	assert(hp_get_cap(&hp) == 4);
	hp_delete(&hp);
	assert(hp_get_cap(NULL) == 0);
}
NO_COV void test_cm_hp_get_current_size(void) {
	t_cm_hp hp;
	int     a = 0x42;
	int     b = 0x40;

	hp_init(&hp, 4, sizeof(int), cb_cmp_int);
	assert(hp_get_current_size(&hp) == 0);
	assert(hp_min_push(&hp, &a) == true);
	assert(hp_get_current_size(&hp) == 1);
	assert(hp_min_push(&hp, &b) == true);
	assert(hp_get_current_size(&hp) == 2);
	hp_delete(&hp);
	assert(hp_get_current_size(NULL) == 0);
}
NO_COV void test_cm_hp_get_elem_size(void) {
	t_cm_hp hp;

	hp_init(&hp, 4, sizeof(int), cb_cmp_int);
	assert(hp_get_elem_size(&hp) == sizeof(int));
	hp_delete(&hp);
	assert(hp_get_elem_size(NULL) == 0);

	hp_init(&hp, 4, sizeof(double), cb_cmp_int);
	assert(hp_get_elem_size(&hp) == sizeof(double));
	hp_delete(&hp);
}
NO_COV void test_cm_hp_get_elems_array(void) {
	t_cm_hp hp;

	hp_init(&hp, 4, sizeof(int), cb_cmp_int);
	assert(hp_get_elems_array(&hp) == hp.elems);
	hp_delete(&hp);
	assert(hp_get_elems_array(NULL) == NULL);
}
NO_COV void test_cm_hp_at(void) {
	t_cm_hp       hp;
	int           a = 0x40;
	int           b = 0x42;
	int           dummy[2];
	const t_cm_hp no_elems = {.nb       = 2,
	                          .capacity = 4,
	                          .sz_e     = sizeof(int),
	                          .elems    = NULL,
	                          .cmp      = cb_cmp_int};
	const t_cm_hp no_sz    = {
	       .nb = 2, .capacity = 4, .sz_e = 0, .elems = dummy, .cmp = cb_cmp_int};

	hp_init(&hp, 4, sizeof(int), cb_cmp_int);
	assert(hp_at(&hp, 0) == NULL);
	assert(hp_min_push(&hp, &a) == true);
	assert(hp_min_push(&hp, &b) == true);
	assert(hp_at(&hp, 0) == hp.elems);
	assert(*(int *)hp_at(&hp, 0) == 0x40);
	assert(*(int *)hp_at(&hp, 1) == 0x42);
	assert(hp_at(&hp, 2) == NULL);
	assert(hp_at(NULL, 0) == NULL);
	hp_delete(&hp);

	assert(hp_at(&no_elems, 0) == NULL);
	assert(hp_at(&no_sz, 0) == NULL);
}
