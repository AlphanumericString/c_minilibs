#pragma once

#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "da/dynamic_array.h"

#define ASSERT_DA_STATE(da, sz, cap, nb)                                       \
	do {                                                                       \
		assert((da).sz_e == (sz));                                             \
		assert((da).cap_e == (cap));                                           \
		assert((da).nb_e == (nb));                                             \
		assert((da).array != NULL);                                            \
	} while (0);

extern const int arr_3[];
extern const int arr_5[];
extern const int arr_2_5[];

// Helper callbacks
bool  cb_true(void *elem, void *arg);
bool  cb_dump_int(void *e, void *a);
bool  cb_filter_strict_inf(void *e, void *a);
bool  cb_add_one(void *elem, void *arg);
bool  cb_false(void *elem, void *arg);
bool  cb_it_true(void *elem, void *arg, size_t pos, bool last);
bool  cb_it_add_it_to_arg(void *elem, void *arg, size_t pos, bool last);
bool  cb_empty_deletor(void *elem, void *arg);
void *cb_merger(void *elem, void *arg);
int   cb_cmp(const void *a, const void *b);

// create
void test_da_init(void);
void test_da_ninit(void);
void test_da_from_array(void);
void test_da_new(void);

// dup
void test_da_dup(void);
void test_da_ndup(void);
void test_da_dup_to(void);
void test_da_ndup_to(void);

// clear
void test_da_clr(void);

// destroy
void test_da_destroy(void);
void test_da_free(void);

// add
void test_da_add(void);
void test_da_cat_array(void);

// grow
void test_da_grow(void);
void test_da_grow_for_size(void);

// shrink
void test_da_shrink(void);
void test_da_shrink_min(void);

// get
void test_da_at(void);
void test_da_get(void);
void test_da_nget(void);
void test_da_array(void);

// set
void test_da_set(void);
void test_da_nset(void);

// swap
void test_da_swap(void);
void test_da_nswap(void);

// rm
void test_da_rm_order(void);
void test_da_rms_order(void);
void test_da_rm(void);
void test_da_rms(void);

// convert
void test_da_to_array(void);
void test_da_to_loc_array(void);

// apply
void test_da_apply(void);
void test_da_napply(void);
void test_da_apply_it(void);
void test_da_napply_it(void);

// fold
void test_da_fold(void);
void test_da_nfold(void);
void test_da_reduce(void);
void test_da_nreduce(void);

// filter
void test_da_filter(void);
void test_da_nfilter(void);
void test_da_filter_out(void);
void test_da_nfilter_out(void);

// advance
void test_da_advance(void);

// reverse
void test_da_reverse(void);
void test_da_nreverse(void);
void test_da_b_nreverse(void);

// find
void test_da_find(void);
void test_da_nfind(void);
void test_da_find_get(void);

// sort
void test_da_sort(void);
void test_da_nsort(void);
