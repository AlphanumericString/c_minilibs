#include <assert.h>
#include <stdlib.h>

#include "hp_tests.h"

static int *mk_ints(void) {
	int *arr = malloc(sizeof(int) * 6);

	assert(arr != NULL);
	arr[0] = 42;
	arr[1] = 17;
	arr[2] = 99;
	arr[3] = 3;
	arr[4] = 56;
	arr[5] = 8;
	return (arr);
}

NO_COV void test_cm_hp_min_init_from_array(void) {
	int    *arr   = mk_ints();
	int     ord[] = {3, 8, 17, 42, 56, 99};
	t_cm_hp h;
	int     out;

	assert(hp_min_init_from_array(&h, arr, 6, sizeof(int), cb_cmp_int) == &h);
	assert(h.nb == 6);
	assert(h.capacity == 6);
	assert(h.elems == arr);
	assert(h.sz_e == sizeof(int));
	assert(h.cmp == cb_cmp_int);
	assert(hp_min_is_valid(&h) == true);
	for (size_t i = 0; i < 6; i++) {
		assert(hp_min_pop(&h, &out) == true);
		assert(out == ord[i]);
	}
	hp_delete(&h);

	assert(hp_min_init_from_array(NULL, arr = mk_ints(), 6, sizeof(int),
	                              cb_cmp_int) == NULL);
	free(arr);
	assert(hp_min_init_from_array(&h, NULL, 6, sizeof(int), cb_cmp_int) ==
	       NULL);
	assert(hp_min_init_from_array(&h, arr = mk_ints(), 0, sizeof(int),
	                              cb_cmp_int) == NULL);
	free(arr);
	assert(hp_min_init_from_array(&h, arr = mk_ints(), 6, 0, cb_cmp_int) ==
	       NULL);
	free(arr);
	assert(hp_min_init_from_array(&h, arr = mk_ints(), 6, sizeof(int), NULL) ==
	       NULL);
	free(arr);
}

NO_COV void test_cm_hp_max_init_from_array(void) {
	int    *arr   = mk_ints();
	int     ord[] = {99, 56, 42, 17, 8, 3};
	t_cm_hp h;
	int     out;

	assert(hp_max_init_from_array(&h, arr, 6, sizeof(int), cb_cmp_int) == &h);
	assert(h.nb == 6);
	assert(h.capacity == 6);
	assert(h.elems == arr);
	assert(h.sz_e == sizeof(int));
	assert(h.cmp == cb_cmp_int);
	assert(hp_max_is_valid(&h) == true);
	for (size_t i = 0; i < 6; i++) {
		assert(hp_max_pop(&h, &out) == true);
		assert(out == ord[i]);
	}
	hp_delete(&h);

	assert(hp_max_init_from_array(NULL, arr = mk_ints(), 6, sizeof(int),
	                              cb_cmp_int) == NULL);
	free(arr);
	assert(hp_max_init_from_array(&h, NULL, 6, sizeof(int), cb_cmp_int) ==
	       NULL);
	assert(hp_max_init_from_array(&h, arr = mk_ints(), 0, sizeof(int),
	                              cb_cmp_int) == NULL);
	free(arr);
	assert(hp_max_init_from_array(&h, arr = mk_ints(), 6, 0, cb_cmp_int) ==
	       NULL);
	free(arr);
	assert(hp_max_init_from_array(&h, arr = mk_ints(), 6, sizeof(int), NULL) ==
	       NULL);
	free(arr);
}

NO_COV void test_cm_hp_min_new_from_array(void) {
	int     *arr   = mk_ints();
	int      ord[] = {3, 8, 17, 42, 56, 99};
	t_cm_hp *h;
	int      out;

	alloc_reset_counter();
	h = hp_min_new_from_array(arr, 6, sizeof(int), cb_cmp_int);
	assert(nb_alloc == 1);
	assert(h != NULL);
	assert(h->nb == 6);
	assert(h->capacity == 6);
	assert(h->elems == arr);
	assert(hp_min_is_valid(h) == true);
	for (size_t i = 0; i < 6; i++) {
		assert(hp_min_pop(h, &out) == true);
		assert(out == ord[i]);
	}
	hp_free(&h);
	assert(h == NULL);

	assert(hp_min_new_from_array(NULL, 6, sizeof(int), cb_cmp_int) == NULL);
	assert(hp_min_new_from_array(arr = mk_ints(), 0, sizeof(int), cb_cmp_int) ==
	       NULL);
	free(arr);
	assert(hp_min_new_from_array(arr = mk_ints(), 6, 0, cb_cmp_int) == NULL);
	free(arr);
	assert(hp_min_new_from_array(arr = mk_ints(), 6, sizeof(int), NULL) ==
	       NULL);
	free(arr);

	arr = mk_ints();
	alloc_reset_counter();
	alloc_set_call_limit(0);
	assert(hp_min_new_from_array(arr, 6, sizeof(int), cb_cmp_int) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	free(arr);
}

NO_COV void test_cm_hp_max_new_from_array(void) {
	int     *arr   = mk_ints();
	int      ord[] = {99, 56, 42, 17, 8, 3};
	t_cm_hp *h;
	int      out;

	alloc_reset_counter();
	h = hp_max_new_from_array(arr, 6, sizeof(int), cb_cmp_int);
	assert(nb_alloc == 1);
	assert(h != NULL);
	assert(h->nb == 6);
	assert(h->capacity == 6);
	assert(h->elems == arr);
	assert(hp_max_is_valid(h) == true);
	for (size_t i = 0; i < 6; i++) {
		assert(hp_max_pop(h, &out) == true);
		assert(out == ord[i]);
	}
	hp_free(&h);
	assert(h == NULL);

	assert(hp_max_new_from_array(NULL, 6, sizeof(int), cb_cmp_int) == NULL);
	assert(hp_max_new_from_array(arr = mk_ints(), 0, sizeof(int), cb_cmp_int) ==
	       NULL);
	free(arr);
	assert(hp_max_new_from_array(arr = mk_ints(), 6, 0, cb_cmp_int) == NULL);
	free(arr);
	assert(hp_max_new_from_array(arr = mk_ints(), 6, sizeof(int), NULL) ==
	       NULL);
	free(arr);

	arr = mk_ints();
	alloc_reset_counter();
	alloc_set_call_limit(0);
	assert(hp_max_new_from_array(arr, 6, sizeof(int), cb_cmp_int) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	free(arr);
}
