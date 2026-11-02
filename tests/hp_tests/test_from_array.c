#include <assert.h>
#include <stdlib.h>

#include "hp_tests.h"

static int cmp_rawptr(const void *a, const void *b) {
	if ((size_t)a < (size_t)b) return (-1);
	else if ((size_t)a > (size_t)b) return (1);
	return (0);
}

NO_COV void test_hp_init_from_array(void) {
	t_hp   hp;
	void **arr  = malloc(sizeof(void *) * 5);
	void **arr2 = malloc(sizeof(void *) * 5);

	assert(arr != NULL);
	assert(arr2 != NULL);
	assert(hp_init_from_array(&hp, arr, 5, cmp_rawptr) == &hp);
	assert(hp.elems == arr);
	assert(hp.capacity == 5);
	assert(hp.nb == 0);
	assert(hp.cmp == cmp_rawptr);
	hp_max_push(&hp, (void *)0x42);
	hp_max_push(&hp, (void *)0x40);
	assert(hp_max_pop(&hp) == (void *)0x42);
	hp_delete(&hp);

	assert(hp_init_from_array(NULL, arr2, 5, cmp_rawptr) == NULL);
	assert(hp_init_from_array(&hp, NULL, 5, cmp_rawptr) == NULL);
	assert(hp_init_from_array(&hp, arr2, 0, cmp_rawptr) == NULL);
	assert(hp_init_from_array(&hp, arr2, 5, NULL) == NULL);
	free(arr2);
}
NO_COV void test_hp_new_from_array(void) {
	t_hp  *hp;
	void **arr  = malloc(sizeof(void *) * 5);
	void **arr2 = malloc(sizeof(void *) * 5);

	assert(arr != NULL);
	assert(arr2 != NULL);
	alloc_reset_counter();
	hp = hp_new_from_array(arr, 5, cmp_rawptr);
	assert(nb_alloc == 1);
	assert(hp != NULL);
	assert(hp->elems == arr);
	assert(hp->capacity == 5);
	assert(hp->nb == 0);
	assert(hp->cmp == cmp_rawptr);
	hp_free(&hp);
	assert(hp == NULL);

	assert(hp_new_from_array(NULL, 5, cmp_rawptr) == NULL);
	assert(hp_new_from_array(arr2, 0, cmp_rawptr) == NULL);
	assert(hp_new_from_array(arr2, 5, NULL) == NULL);
	alloc_reset_counter();
	alloc_set_call_limit(0);
	assert(hp_new_from_array(arr2, 5, cmp_rawptr) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	free(arr2);
}
