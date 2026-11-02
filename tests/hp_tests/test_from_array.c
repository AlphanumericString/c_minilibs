#include <assert.h>
#include <stdlib.h>

#include "hp_tests.h"

NO_COV void test_hp_init_from_array(void) {
	t_hp hp;
	int *arr  = malloc(sizeof(int) * 5);
	int *arr2 = malloc(sizeof(int) * 5);
	int  a    = 0x42;
	int  b    = 0x40;
	int  out;

	assert(arr != NULL);
	assert(arr2 != NULL);
	assert(hp_init_from_array(&hp, arr, 5, sizeof(int), cb_cmp_int) == &hp);
	assert(hp.elems == arr);
	assert(hp.capacity == 5);
	assert(hp.sz_e == sizeof(int));
	assert(hp.nb == 0);
	assert(hp.cmp == cb_cmp_int);
	assert(hp_max_push(&hp, &a) == true);
	assert(hp_max_push(&hp, &b) == true);
	assert(hp_max_pop(&hp, &out) == true);
	assert(out == 0x42);
	hp_delete(&hp);

	assert(hp_init_from_array(NULL, arr2, 5, sizeof(int), cb_cmp_int) == NULL);
	assert(hp_init_from_array(&hp, NULL, 5, sizeof(int), cb_cmp_int) == NULL);
	assert(hp_init_from_array(&hp, arr2, 0, sizeof(int), cb_cmp_int) == NULL);
	assert(hp_init_from_array(&hp, arr2, 5, 0, cb_cmp_int) == NULL);
	assert(hp_init_from_array(&hp, arr2, 5, sizeof(int), NULL) == NULL);
	free(arr2);
}
NO_COV void test_hp_new_from_array(void) {
	t_hp *hp;
	int  *arr  = malloc(sizeof(int) * 5);
	int  *arr2 = malloc(sizeof(int) * 5);

	assert(arr != NULL);
	assert(arr2 != NULL);
	alloc_reset_counter();
	hp = hp_new_from_array(arr, 5, sizeof(int), cb_cmp_int);
	assert(nb_alloc == 1);
	assert(hp != NULL);
	assert(hp->elems == arr);
	assert(hp->capacity == 5);
	assert(hp->sz_e == sizeof(int));
	assert(hp->nb == 0);
	assert(hp->cmp == cb_cmp_int);
	hp_free(&hp);
	assert(hp == NULL);

	assert(hp_new_from_array(NULL, 5, sizeof(int), cb_cmp_int) == NULL);
	assert(hp_new_from_array(arr2, 0, sizeof(int), cb_cmp_int) == NULL);
	assert(hp_new_from_array(arr2, 5, 0, cb_cmp_int) == NULL);
	assert(hp_new_from_array(arr2, 5, sizeof(int), NULL) == NULL);
	alloc_reset_counter();
	alloc_set_call_limit(0);
	assert(hp_new_from_array(arr2, 5, sizeof(int), cb_cmp_int) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	free(arr2);
}
