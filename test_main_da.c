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

#define FILL_DA_TO(da, up_to)                                                  \
	do {                                                                       \
		int a = 0;                                                             \
		for (; a <= (up_to); a++)                                              \
			da_add(da, &a);                                                    \
	} while (0);

const int arr_3[]   = {1, 2, 3};
const int arr_5[]   = {1, 2, 3, 4, 5};
const int arr_2_5[] = {2, 3, 4, 5};

// Helper callbacks
bool cb_true(void *elem, void *arg) {
	(void)elem;
	(void)arg;
	return true;
}
bool cb_dump_int(void *e, void *a) {
	(void)a;
	return (printf("%d\n", *(int *)e), true);
}
bool cb_filter_strict_inf(void *e, void *a) {
	int _e = *(int *)e;
	int _a = *(int *)a;

	return (_e) < (_a);
}
bool cb_add_one(void *elem, void *arg) {
	(void)elem;
	(void)arg;

	*(int *)elem += 1;
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
bool cb_it_add_it_to_arg(void *elem, void *arg, size_t pos, bool last) {
	(void)last;

	if ((unsigned long)arg <= pos) return (false);
	*(int *)elem += (int)pos;
	return (true);
}

bool cb_empty_deletor(void *elem, void *arg) {
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
	t_da da, dup;

	da_init(&da, sizeof(int));
	int a = 1;
	da_add(&da, &a);

	dup = da_dup(&da);
	ASSERT_DA_STATE(dup, da.sz_e, da.nb_e, da.nb_e);
	assert(*(int *)da_at(&dup, 0) == *(int *)da_at(&da, 0));

	da_destroy(&da);
	da_destroy(&dup);
}
void test_da_ndup(void) {
	t_da da, dup;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));

	dup = da_ndup(&da, 2);
	ASSERT_DA_STATE(dup, da.sz_e, 2, 2);
	assert(*(int *)da_at(&dup, 0) == *(int *)da_at(&da, 0));
	assert(*(int *)da_at(&dup, 1) == *(int *)da_at(&da, 1));

	da_destroy(&da);
	da_destroy(&dup);
}
void test_da_dup_to(void) {
	t_da da, dst;
	int  a;

	da_init(&da, sizeof(int));
	da_init(&dst, sizeof(int));
	a = 1;
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
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));

	da_ndup_to(&dst, 2, &da);
	ASSERT_DA_STATE(dst, da.sz_e, C_MINI_DA__NB_ELEM_DFLT, 2);
	assert(*(int *)da_at(&dst, 0) == *(int *)da_at(&da, 0));
	assert(*(int *)da_at(&dst, 1) == *(int *)da_at(&da, 1));

	da_destroy(&da);
	da_destroy(&dst);
}

// clear
void test_da_clr(void) {
	t_da   da;
	size_t sz;
	size_t cap;
	void  *arr;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));

	sz  = da.sz_e;
	cap = da.cap_e;
	arr = da.array;
	da_clr(&da);
	ASSERT_DA_STATE(da, sz, cap, 0);
	assert(da.array == arr);

	da_destroy(&da);
}

// destroy
void test_da_destroy(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	ASSERT_DA_STATE(da, da.sz_e, da.cap_e, 5);

	da_destroy(&da);
	assert(da.sz_e == 0);
	assert(da.cap_e == 0);
	assert(da.nb_e == 0);
	assert(da.array == NULL);
}

// add
void test_da_add(void) {
	t_da da;
	int  a;

	da_init(&da, sizeof(int));

	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	a = 5;
	da_add(&da, &a);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 1);
	assert(*(int *)(da.array) == a);

	da_destroy(&da);
}
void test_da_cat_array(void) {
	t_da da;
	int  arr[] = {1, 2, 3};

	da_init(&da, sizeof(int));
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
	int  a;

	da_init(&da, sizeof(int));
	a = 1;
	da_add(&da, &a);

	assert(*(int *)da_at(&da, 0) == 1);

	da_destroy(&da);
}
void test_da_get(void) {
	t_da da;
	int  a, out;

	da_init(&da, sizeof(int));
	a = 1;
	da_add(&da, &a);

	da_get(&da, 0, &out);
	assert(out == 1);

	da_destroy(&da);
}
void test_da_nget(void) {
	t_da da;
	int  buff[2];
	int  out;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));

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
	da_cat_array(&da, (int[]){1, 43, 4, 5, 223}, 5, sizeof(int));
	assert(da_array(&da) == da.array);

	da_destroy(&da);
}

// set
void test_da_set(void) {
	t_da da;
	int a = 1;
	int b = 2;

	da_init(&da, sizeof(int));
	da_add(&da, &a);
	da_set(&da, 0, &b);

	assert(*(int *)da_at(&da, 0) == 2);

	da_destroy(&da);
}
void test_da_nset(void) {
	t_da da;
	int b       = 2;
	int buff[2] = {3, 4};

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));

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
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));

	da_swap(&da, 0, 1);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 1);

	da_destroy(&da);
}
void test_da_nswap(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

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
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

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
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

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
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	da_rm(&da, 0);
	assert(da.nb_e == 4);
	assert(*(int *)da_at(&da, 0) != 1);

	da_destroy(&da);
}
void test_da_rms(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

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
	int *arr;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	arr = (int *)da_to_array(&da);
	assert(arr != NULL);
	assert(arr[0] == 1);
	assert(arr[1] == 2);
	assert(arr[2] == 3);
	assert(arr[3] == 4);
	assert(arr[4] == 5);

	free(arr);
	da_destroy(&da);
}
void test_da_to_loc_array(void) {
	t_da da;
	int  a      = 1;
	int  arr[5] = {42, 42, 42, 42, 42};

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	da_to_loc_array(&da, arr);
	assert(arr != NULL);
	assert(arr[0] == 1);
	assert(arr[1] == 42);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	da_to_loc_array(&da, arr);
	assert(arr != NULL);
	assert(arr[0] == 1);
	assert(arr[1] == 2);
	assert(arr[2] == 3);
	assert(arr[3] == 4);
	assert(arr[4] == 5);

	da_destroy(&da);
}

// apply
void test_da_apply(void) {
	t_da da;
	int a = 1;

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	assert(*(int *)da_at(&da, 0) == 1);
	assert(da_apply(&da, cb_true, NULL) == NULL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(da_apply(&da, cb_true, (void *)0xBEEFDEAD) == (void *)0xBEEFDEAD);
	assert(da_apply(&da, cb_add_one, (void *)0xDEAD) == (void *)0xDEAD);
	assert(*(int *)da_at(&da, 0) == 2);

	da_destroy(&da);
}
void test_da_napply(void) {
	t_da da;
	int a = 1;

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	assert(*(int *)da_at(&da, 0) == 1);
	assert(da_napply(&da, 1, cb_add_one, NULL) == NULL);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(da_napply(&da, 14, cb_add_one, (void *)0xDEAD) == (void *)0xDEAD);
	assert(*(int *)da_at(&da, 0) == 3);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	da_napply(&da, 3, cb_add_one, NULL);
	assert(*(int *)da_at(&da, 0) == 4);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);

	da_destroy(&da);
}
void test_da_apply_it(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	da_apply_it(&da, cb_it_add_it_to_arg, (void *)3UL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 5);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);

	da_destroy(&da);
}
void test_da_napply_it(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	da_napply_it(&da, 1, cb_it_true, (void *)3UL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	da_napply_it(&da, 2, cb_it_add_it_to_arg, (void *)3UL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	da_napply_it(&da, 3, cb_it_add_it_to_arg, (void *)3UL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 5);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	da_napply_it(&da, 4, cb_it_add_it_to_arg, (void *)3UL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 5);
	assert(*(int *)da_at(&da, 2) == 7);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);

	da_destroy(&da);
}

// fold
void test_da_fold(void) {
	t_da da;
	int  sum = 0;
	int  a   = 1;

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	da_fold(&da, cb_merger, &sum);
	assert(sum == 1);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	sum = 0;
	da_fold(&da, cb_merger, &sum);
	assert(sum == (1 + 2 + 3 + 4 + 5));

	da_destroy(&da);
}
void test_da_nfold(void) {
	t_da da;
	int  sum = 0;
	int  a   = 1;

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	da_nfold(&da, 1, cb_merger, &sum);
	assert(sum == 1);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	sum = 0;
	da_nfold(&da, 1, cb_merger, &sum);
	assert(sum == 1);
	sum = 0;
	da_nfold(&da, 3, cb_merger, &sum);
	assert(sum == (1 + 2 + 3));
	sum = 42;
	da_nfold(&da, -1, cb_merger, &sum);
	assert(sum == (42 + (1 + 2 + 3 + 4 + 5)));

	da_destroy(&da);
}
void test_da_reduce(void) {
	t_da da;
	int  a = 1;
	int *res;

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	res = da_reduce(&da, cb_merger);
	assert(res != NULL);
	assert(*res == 1);
	free(res);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	res = da_reduce(&da, cb_merger);
	assert(res != NULL);
	assert(*res == (1 + 2 + 3 + 4 + 5));
	free(res);

	da_destroy(&da);
}
void test_da_nreduce(void) {
	t_da da;
	int  a = 1;
	int *res;

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	res = da_nreduce(&da, 1, cb_merger);
	assert(res != NULL);
	assert(*res == 1);
	free(res);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	res = da_nreduce(&da, 3, cb_merger);
	assert(res != NULL);
	assert(*res == (1 + 2 + 3));
	free(res);
	res = da_nreduce(&da, -1, cb_merger);
	assert(res != NULL);
	assert(*res == (1 + 2 + 3 + 4 + 5));
	free(res);

	da_destroy(&da);
}

// filter
void test_da_filter(void) {
	t_da da;
	int  a     = 1;
	int  bound = 3;

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	da_filter(&da, cb_filter_strict_inf, &bound, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 1);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 5);
	da_filter(&da, cb_filter_strict_inf, &bound, cb_empty_deletor);
	da_sort(&da, cb_cmp);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 2);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);

	da_destroy(&da);
}
void test_da_nfilter(void) {
	t_da da;
	int  a     = 1;
	int  bound = 3;

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	da_nfilter(&da, 1, cb_true, NULL, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 1);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 5);
	da_nfilter(&da, 4, cb_filter_strict_inf, &bound, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 3);
	assert(*(int *)da_at(&da, 2) == 5);

	da_destroy(&da);
}
void test_da_filter_out(void) {
	t_da da;
	int  a     = 1;
	int  bound = 3;

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	da_filter_out(&da, cb_filter_strict_inf, &bound, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 0);
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	da_filter_out(&da, cb_filter_strict_inf, &bound, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 3);
	da_sort(&da, cb_cmp);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 5);

	da_destroy(&da);
}
void test_da_nfilter_out(void) {
	t_da da;
	int  a     = 1;
	int  bound = 3;

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	da_nfilter_out(&da, 1, cb_true, &bound, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 0);
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	da_nfilter_out(&da, 1, cb_filter_strict_inf, &bound, cb_empty_deletor);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 4);
	da_sort(&da, cb_cmp);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	assert(*(int *)da_at(&da, 3) == 5);

	da_destroy(&da);
}

// advance
void test_da_advance(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 5);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	da_advance(&da, 1, cb_empty_deletor, NULL);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 4);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	da_advance(&da, 2, cb_empty_deletor, NULL);
	ASSERT_DA_STATE(da, sizeof(int), da.cap_e, 2);
	assert(*(int *)da_at(&da, 0) == 4);
	assert(*(int *)da_at(&da, 1) == 5);

	da_destroy(&da);
}

// reverse
void test_da_reverse(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	da_reverse(&da);
	assert(*(int *)da_at(&da, 0) == 5);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 2);
	assert(*(int *)da_at(&da, 4) == 1);

	da_destroy(&da);
}
void test_da_nreverse(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	da_nreverse(&da, 2);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 1);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);

	da_destroy(&da);
}
void test_da_b_nreverse(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	da_b_nreverse(&da, 2);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 5);
	assert(*(int *)da_at(&da, 4) == 4);

	da_destroy(&da);
}

// find
void test_da_find(void) {
	t_da  da;
	void *p;
	int   value = 1;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	p = da_find(&da, cb_cmp, &value);
	assert(p != NULL);
	assert(*(int *)p == 1);
	assert(p == da_at(&da, 0));
	value = 3;
	p     = da_find(&da, cb_cmp, &value);
	assert(p != NULL);
	assert(*(int *)p == 3);
	assert(p == da_at(&da, 2));
	value = 42;
	p     = da_find(&da, cb_cmp, &value);
	assert(p == NULL);

	da_destroy(&da);
}
void test_da_nfind(void) {
	t_da  da;
	void *p;
	int   value = 1;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	p = da_nfind(&da, 1, cb_cmp, &value);
	assert(p != NULL);
	assert(*(int *)p == 1);
	assert(p == da_at(&da, 0));
	value = 3;
	p     = da_nfind(&da, 1, cb_cmp, &value);
	assert(p == NULL);
	p = da_nfind(&da, 5, cb_cmp, &value);
	assert(p != NULL);
	assert(*(int *)p == 3);
	assert(p == da_at(&da, 2));
	value = 42;
	p     = da_nfind(&da, -1, cb_cmp, &value);
	assert(p == NULL);

	da_destroy(&da);
}
void test_da_find_get(void) {
	t_da da;
	int  out;
	int  value = 1;
	bool ret;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	ret = da_find_get(&da, cb_cmp, &value, &out);
	assert(ret == true);
	assert(out == 1);
	value = 3;
	ret   = da_find_get(&da, cb_cmp, &value, &out);
	assert(ret == true);
	assert(out == 3);
	value = 4;
	ret   = da_find_get(&da, cb_cmp, &value, &out);
	assert(ret == true);
	assert(out == 4);
	value = 42;
	out   = -3012;
	ret   = da_find_get(&da, cb_cmp, &value, &out);
	assert(ret == false);
	assert(out == -3012);

	da_destroy(&da);
}

// sort
void test_da_sort(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	da_swap(&da, 1, 3);
	da_swap(&da, 2, 1);
	da_swap(&da, 0, 3);

	da_sort(&da, cb_cmp);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);

	da_destroy(&da);
}
void test_da_nsort(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int[]){3, 2, 1, 4, 5}, 5, sizeof(int));

	da_nsort(&da, 2, cb_cmp);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 3);
	da_nsort(&da, 3, cb_cmp);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);

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
