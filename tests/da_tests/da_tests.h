#pragma once

#include <stdbool.h>

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "common_base/tests.h"
#include "da/dynamic_array.h"

#define ASSERT_DA_STATE(da, sz, cap, nb)                                       \
	do {                                                                       \
		assert((da).sz_e == (sz));                                             \
		assert((da).cap_e == (cap));                                           \
		assert((da).nb_e == (nb));                                             \
		assert((da).array != NULL);                                            \
	} while (0);

#define DA_DUMP(x)                                                             \
	do {                                                                       \
		DBG("%s: {.arr=%p, .nb_e=%zu, .cap_e=%zu, .sz_e=%zu}", #x, (x).array,  \
		    (x).nb_e, (x).cap_e, (x).sz_e);                                    \
	} while (0);

extern const int arr_3[];
extern const int arr_5[];
extern const int arr_2_5[];

// Helper callbacks
NO_COV bool  cb_true(void *elem, void *arg);
NO_COV bool  cb_dump_int(void *e, void *a);
NO_COV bool  cb_filter_strict_inf(void *e, void *a);
NO_COV bool  cb_add_one(void *elem, void *arg);
NO_COV bool  cb_false(void *elem, void *arg);
NO_COV bool  cb_it_true(void *elem, void *arg, size_t pos, bool last);
NO_COV bool  cb_it_add_it_to_arg(void *elem, void *arg, size_t pos, bool last);
NO_COV bool  cb_empty_deletor(void *elem, void *arg);
NO_COV void *cb_merger(void *elem, void *arg);
NO_COV int   cb_cmp(const void *a, const void *b);

void test_cm_da_init(void);
void test_cm_da_ninit(void);
void test_cm_da_from_array(void);
void test_cm_da_new(void);

void test_cm_da_dup(void);
void test_cm_da_ndup(void);
void test_cm_da_dup_to(void);
void test_cm_da_ndup_to(void);

void test_cm_da_clr(void);

void test_cm_da_destroy(void);
void test_cm_da_free(void);

void test_cm_da_add(void);
void test_cm_da_cat_array(void);

void test_cm_da_grow(void);
void test_cm_da_grow_for_size(void);

void test_cm_da_is_valid(void);
void test_cm_da_is_not_null(void);
void test_cm_da_is_empty(void);
void test_cm_da_is_full(void);

void test_cm_da_shrink(void);
void test_cm_da_shrink_min(void);

void test_cm_da_at(void);
void test_cm_da_get(void);
void test_cm_da_nget(void);
void test_cm_da_array(void);

void test_cm_da_set(void);
void test_cm_da_nset(void);

void test_cm_da_swap(void);
void test_cm_da_nswap(void);

void test_cm_da_rm_order(void);
void test_cm_da_rms_order(void);
void test_cm_da_rm(void);
void test_cm_da_rms(void);

void test_cm_da_to_array(void);
void test_cm_da_to_loc_array(void);

void test_cm_da_apply(void);
void test_cm_da_napply(void);
void test_cm_da_apply_it(void);
void test_cm_da_napply_it(void);

void test_cm_da_fold(void);
void test_cm_da_nfold(void);
void test_cm_da_reduce(void);
void test_cm_da_nreduce(void);

void test_cm_da_filter(void);
void test_cm_da_nfilter(void);
void test_cm_da_filter_out(void);
void test_cm_da_nfilter_out(void);

void test_cm_da_advance(void);

void test_cm_da_reverse(void);
void test_cm_da_nreverse(void);
void test_cm_da_b_nreverse(void);

void test_cm_da_find(void);
void test_cm_da_nfind(void);
void test_cm_da_find_get(void);

void test_cm_da_sort(void);
void test_cm_da_nsort(void);
