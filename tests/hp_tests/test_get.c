#include <assert.h>

#include "hp_tests.h"

static int cmp_rawptr(const void *a, const void *b) {
	if ((size_t)a < (size_t)b) return (-1);
	else if ((size_t)a > (size_t)b) return (1);
	return (0);
}

NO_COV void test_hp_get_cmp(void) {
	t_hp hp;

	hp_init(&hp, 4, cmp_rawptr);
	assert(hp_get_cmp(&hp) == cmp_rawptr);
	hp_delete(&hp);
	assert(hp_get_cmp(NULL) == NULL);
}
NO_COV void test_hp_get_cap(void) {
	t_hp hp;

	hp_init(&hp, 4, cmp_rawptr);
	assert(hp_get_cap(&hp) == 4);
	hp_delete(&hp);
	assert(hp_get_cap(NULL) == 0);
}
NO_COV void test_hp_get_current_size(void) {
	t_hp hp;

	hp_init(&hp, 4, cmp_rawptr);
	assert(hp_get_current_size(&hp) == 0);
	hp_min_push(&hp, (void *)0x42);
	assert(hp_get_current_size(&hp) == 1);
	hp_min_push(&hp, (void *)0x40);
	assert(hp_get_current_size(&hp) == 2);
	hp_delete(&hp);
	assert(hp_get_current_size(NULL) == 0);
}
NO_COV void test_hp_get_elems_array(void) {
	t_hp hp;

	hp_init(&hp, 4, cmp_rawptr);
	assert(hp_get_elems_array(&hp) == hp.elems);
	hp_delete(&hp);
	assert(hp_get_elems_array(NULL) == NULL);
}
