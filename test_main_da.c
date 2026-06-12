#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "./da/dynamic_array.h"

#define ASSERT_DA_STATE(da, sz, cap, nb)                                       \
	do {                                                                       \
		assert((da).sz_e == (sz));                                             \
		assert((da).cap_e == (cap));                                           \
		assert((da).nb_e == (nb));                                             \
		assert((da).array != NULL);                                            \
	} while (0);

// Helper callbacks
bool cb_true(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return true;
}
bool cb_false(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return false;
}
bool cb_it_true(void *elem, void *arg, size_t pos, bool last) {
	(void)elem;
	(void)arg;
	(void)pos;
	(void)last;
	return true;
}

bool cb_deletor(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return (true);
}
void *cb_merger(void *elem, void *arg) {
	*(int *)arg += *(int *)elem;
	return arg;
}
int cb_cmp(const void *a, const void *b) {
	return (*(int *)a - *(int *)b);
}

// create
void test_da_init(void) {
	t_da da;

	da_init(&da, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);

	da_destroy(&da);
}
void test_da_ninit(void) {
	t_da da;

	da_ninit(&da, 10, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), 10, 0);

	da_destroy(&da);
}
void test_da_from_array(void) {
	t_da da;
	int *arr = calloc(5, sizeof(int));

	da_from_array(&da, arr, 5, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), 5, 0);
	assert(da.array == arr);

	da_destroy(&da);
}
void test_da_new(void) {

	t_da *da = da_new(sizeof(int));
	ASSERT_DA_STATE(*da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);

	da_destroy(da);
	free(da);
}

// dup
void test_da_dup(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);

	t_da dup = da_dup(&da);
	ASSERT_DA_STATE(dup, da.sz_e, da.nb_e, da.nb_e);
	assert(*(int *)da_at(&dup, 0) == *(int *)da_at(&da, 0));

	da_destroy(&da);
	da_destroy(&dup);
}
void test_da_ndup(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	a = 2;
	da_add(&da, &a);
	a = 3;
	da_add(&da, &a);

	t_da dup = da_ndup(&da, 2);
	ASSERT_DA_STATE(dup, da.sz_e, 2, 2);
	assert(*(int *)da_at(&dup, 0) == *(int *)da_at(&da, 0));
	assert(*(int *)da_at(&dup, 1) == *(int *)da_at(&da, 1));

	da_destroy(&da);
	da_destroy(&dup);
}
void test_da_dup_to(void) {
	t_da da, dst;
	da_init(&da, sizeof(int));
	da_init(&dst, sizeof(int));
	int a = 1;
	da_add(&da, &a);

	da_dup_to(&dst, &da);
	ASSERT_DA_STATE(dst, da.sz_e, C_MINI_DA__NB_ELEM_DFLT, da.nb_e);
	assert(*(int *)da_at(&da, 0) == *(int *)da_at(&dst, 0));

	da_destroy(&da);
	da_destroy(&dst);
}
void test_da_ndup_to(void) {
	t_da da, dst;
	da_init(&da, sizeof(int));
	da_init(&dst, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	a = 2;
	da_add(&da, &a);
	a = 3;
	da_add(&da, &a);

	da_ndup_to(&dst, 2, &da);
	ASSERT_DA_STATE(dst, da.sz_e, C_MINI_DA__NB_ELEM_DFLT, 2);
	assert(*(int *)da_at(&dst, 0) == *(int *)da_at(&da, 0));
	assert(*(int *)da_at(&dst, 1) == *(int *)da_at(&da, 1));

	da_destroy(&da);
	da_destroy(&dst);
}

// clear
void test_da_clr(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	a = 2;
	da_add(&da, &a);
	a = 3;
	da_add(&da, &a);
	size_t sz  = da.sz_e;
	size_t cap = da.cap_e;
	void  *arr = da.array;

	da_clr(&da);
	ASSERT_DA_STATE(da, sz, cap, 0);
	assert(da.array == arr);

	da_destroy(&da);
}

// destroy
void test_da_destroy(void) {
	t_da da;
	da_init(&da, sizeof(int));

	da_destroy(&da);
	assert(da.sz_e == 0);
	assert(da.cap_e == 0);
	assert(da.nb_e == 0);
	assert(da.array == NULL);
}

// add
void test_da_add(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 5;

	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_add(&da, &a);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 1);
	assert(*(int *)(da.array) == a);

	da_destroy(&da);
}
void test_da_cat_array(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int arr[] = {1, 2, 3};
	da_cat_array(&da, arr, 3, sizeof(int));
	da_destroy(&da);
}

// grow
void test_da_grow(void) {
	t_da da;
	da_init(&da, sizeof(int));

	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_grow(&da);
	ASSERT_DA_STATE(da, sizeof(int),
	                C_MINI_DA__NB_ELEM_DFLT * C_MINI_DA__GROWTH, 0);

	da_destroy(&da);
}
void test_da_grow_for_size(void) {
	t_da da;
	da_init(&da, sizeof(int));

	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_grow_for_size(&da, 20);
	assert(da.cap_e >= 20);
	assert(da.sz_e == sizeof(int));
	assert(da.nb_e == 0);
	assert(da.array != NULL);

	da_destroy(&da);
}

// shrink
void test_da_shrink(void) {
	t_da da;
	da_init(&da, sizeof(int));

	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_shrink(&da);
	ASSERT_DA_STATE(da, sizeof(int), 0, 0);

	da_destroy(&da);
}
void test_da_shrink_min(void) {
	t_da da;
	da_init(&da, sizeof(int));

	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_shrink_min(&da, 5);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);

	da_destroy(&da);
}

// get
void test_da_at(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);

	assert(*(int *)da_at(&da, 0) == 1);

	da_destroy(&da);
}
void test_da_get(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1, out;
	da_add(&da, &a);

	da_get(&da, 0, &out);
	assert(out == 1);

	da_destroy(&da);
}
void test_da_nget(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1, out;
	int buff[2];
	da_add(&da, &a);
	a = 2;
	da_add(&da, &a);
	a = 3;
	da_add(&da, &a);

	da_nget(&da, 1, 0, &out);
	assert(out == 1);
	da_nget(&da, 2, 0, &buff);
	assert(buff[0] == 1);
	assert(buff[1] == 2);

	da_destroy(&da);
}
void test_da_array(void) {
	t_da da;
	da_init(&da, sizeof(int));

	assert(da_array(&da) == da.array);

	da_destroy(&da);
}

// set
void test_da_set(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	int b = 2;
	da_set(&da, 0, &b);

	assert(*(int *)da_at(&da, 0) == 2);

	da_destroy(&da);
}
void test_da_nset(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	a = 2;
	da_add(&da, &a);
	a = 3;
	da_add(&da, &a);

	int b       = 2;
	int buff[2] = {3, 4};
	da_nset(&da, 1, 0, &b);
	assert(*(int *)da_at(&da, 0) == 2);
	da_nset(&da, 2, 0, &buff);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);

	da_destroy(&da);
}

// swap
void test_da_swap(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1, b = 2;
	da_add(&da, &a);
	da_add(&da, &b);

	da_swap(&da, 0, 1);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 1);

	da_destroy(&da);
}
void test_da_nswap(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	a = 2;
	da_add(&da, &a);
	a = 3;
	da_add(&da, &a);
	a = 4;
	da_add(&da, &a);
	a = 5;
	da_add(&da, &a);

	da_nswap(&da, 1, 0, 1); // 2 1 3 4 5
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 1);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	da_nswap(&da, 2, 0, 2); // 3 4 2 1 5
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 2);
	assert(*(int *)da_at(&da, 3) == 1);

	da_destroy(&da);
}

// rm
void test_da_rm_order(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	a = 2;
	da_add(&da, &a);
	a = 3;
	da_add(&da, &a);
	a = 4;
	da_add(&da, &a);
	a = 5;
	da_add(&da, &a);

	da_rm_order(&da, 0);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	assert(*(int *)da_at(&da, 3) == 5);

	da_destroy(&da);
}
void test_da_rms_order(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	a = 2;
	da_add(&da, &a);
	a = 3;
	da_add(&da, &a);
	a = 4;
	da_add(&da, &a);
	a = 5;
	da_add(&da, &a);

	da_rms_order(&da, 0, 1);
	assert(da.nb_e == 4);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	assert(*(int *)da_at(&da, 3) == 5);
	da_rms_order(&da, 1, 3);
	assert(da.nb_e == 2);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 5);
	da_rms_order(&da, 1, -1);
	assert(da.nb_e == 1);
	assert(*(int *)da_at(&da, 0) == 2);

	da_destroy(&da);
}
void test_da_rm(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	a = 2;
	da_add(&da, &a);
	a = 3;
	da_add(&da, &a);
	a = 4;
	da_add(&da, &a);
	a = 5;
	da_add(&da, &a);

	da_rm(&da, 0);
	assert(da.nb_e == 4);
	assert(*(int *)da_at(&da, 0) != 1);

	da_destroy(&da);
}
void test_da_rms(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	a = 2;
	da_add(&da, &a);
	a = 3;
	da_add(&da, &a);
	a = 4;
	da_add(&da, &a);
	a = 5;
	da_add(&da, &a);

	da_rms(&da, 0, 1);
	assert(da.nb_e == 4);
	assert(*(int *)da_at(&da, 0) != 1);
	da_rms(&da, 0, 2);
	assert(da.nb_e == 2);
	da_rms(&da, 0, -1);
	assert(da.nb_e == 0);

	da_destroy(&da);
}

// convert
void test_da_to_array(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	void *arr = da_to_array(&da);
	free(arr);
	da_destroy(&da);
}
void test_da_to_loc_array(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	int arr[1];
	da_to_loc_array(&da, arr);
	da_destroy(&da);
}

// apply
void test_da_apply(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	da_apply(&da, cb_true, NULL);
	da_destroy(&da);
}
void test_da_napply(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	da_napply(&da, 1, cb_true, NULL);
	da_destroy(&da);
}
void test_da_apply_it(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	da_apply_it(&da, cb_it_true, NULL);
	da_destroy(&da);
}
void test_da_napply_it(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	da_napply_it(&da, 1, cb_it_true, NULL);
	da_destroy(&da);
}

// fold
void test_da_fold(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1, sum = 0;
	da_add(&da, &a);
	da_fold(&da, cb_merger, &sum);
	da_destroy(&da);
}
void test_da_nfold(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1, sum = 0;
	da_add(&da, &a);
	da_nfold(&da, 1, cb_merger, &sum);
	da_destroy(&da);
}
void test_da_reduce(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	void *res = da_reduce(&da, cb_merger);
	free(res);
	da_destroy(&da);
}
void test_da_nreduce(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	void *res = da_nreduce(&da, 1, cb_merger);
	free(res);
	da_destroy(&da);
}

// filter
void test_da_filter(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	da_filter(&da, cb_true, NULL, cb_deletor);
	da_destroy(&da);
}
void test_da_nfilter(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	da_nfilter(&da, 1, cb_true, NULL, cb_deletor);
	da_destroy(&da);
}
void test_da_filter_out(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	da_filter_out(&da, cb_true, NULL, cb_deletor);
	da_destroy(&da);
}
void test_da_nfilter_out(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	da_nfilter_out(&da, 1, cb_true, NULL, cb_deletor);
	da_destroy(&da);
}

// advance
void test_da_advance(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1, b = 2;
	da_add(&da, &a);
	da_add(&da, &b);
	da_advance(&da, 1, cb_deletor, NULL);
	da_destroy(&da);
}

// reverse
void test_da_reverse(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1, b = 2;
	da_add(&da, &a);
	da_add(&da, &b);
	da_reverse(&da);
	da_destroy(&da);
}
void test_da_nreverse(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1, b = 2;
	da_add(&da, &a);
	da_add(&da, &b);
	da_nreverse(&da, 2);
	da_destroy(&da);
}
void test_da_b_nreverse(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1, b = 2;
	da_add(&da, &a);
	da_add(&da, &b);
	da_b_nreverse(&da, 2);
	da_destroy(&da);
}

// find
void test_da_find(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	da_find(&da, cb_true, NULL);
	da_destroy(&da);
}
void test_da_nfind(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);
	da_nfind(&da, 1, cb_true, NULL);
	da_destroy(&da);
}
void test_da_find_get(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 1, out;
	da_add(&da, &a);
	da_find_get(&da, cb_true, NULL, &out);
	da_destroy(&da);
}

// sort
void test_da_sort(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 2, b = 1;
	da_add(&da, &a);
	da_add(&da, &b);
	da_sort(&da, cb_cmp);
	da_destroy(&da);
}
void test_da_nsort(void) {
	t_da da;
	da_init(&da, sizeof(int));
	int a = 2, b = 1;
	da_add(&da, &a);
	da_add(&da, &b);
	da_nsort(&da, 2, cb_cmp);
	da_destroy(&da);
}

int main(void) {
	test_da_init();
	test_da_ninit();
	test_da_from_array();
	test_da_new();
	test_da_dup();
	test_da_ndup();
	test_da_dup_to();
	test_da_ndup_to();
	test_da_clr();
	test_da_destroy();
	test_da_add();
	test_da_cat_array();
	test_da_grow();
	test_da_grow_for_size();
	test_da_shrink();
	test_da_shrink_min();
	test_da_at();
	test_da_get();
	test_da_nget();
	test_da_array();
	test_da_set();
	test_da_nset();
	test_da_swap();
	test_da_nswap();
	test_da_rm_order();
	test_da_rms_order();
	test_da_rm();
	test_da_rms();
	test_da_to_array();
	test_da_to_loc_array();
	test_da_apply();
	test_da_napply();
	test_da_apply_it();
	test_da_napply_it();
	test_da_fold();
	test_da_nfold();
	test_da_reduce();
	test_da_nreduce();
	test_da_filter();
	test_da_nfilter();
	test_da_filter_out();
	test_da_nfilter_out();
	test_da_advance();
	test_da_reverse();
	test_da_nreverse();
	test_da_b_nreverse();
	test_da_find();
	test_da_nfind();
	test_da_find_get();
	test_da_sort();
	test_da_nsort();

	printf("All da tests completed successfully.\n");
	return 0;
}
