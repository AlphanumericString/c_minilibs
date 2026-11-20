#pragma once

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "common_base/tests.h"
#include "hp/heap.h"

#define HP_ORDER_MAX 8

typedef struct s_cm_order_ctx {
	int    vals[HP_ORDER_MAX];
	size_t pos[HP_ORDER_MAX];
	bool   last[HP_ORDER_MAX];
	size_t n;
} t_cm_order_ctx;

// Helper callbacks
NO_COV int   cb_cmp_int(const void *a, const void *b);
NO_COV bool  cb_true(void *e, void *a);
NO_COV bool  cb_false(void *e, void *a);
NO_COV bool  cb_fail_at(void *e, void *a);
NO_COV bool  cb_log_int(void *e, void *a);
NO_COV bool  cb_it_log(void *e, void *a, size_t pos, bool last);
NO_COV bool  cb_it_false(void *e, void *a, size_t pos, bool last);
NO_COV bool  cb_keep_even(void *e, void *a);
NO_COV bool  cb_keep_all(void *e, void *a);
NO_COV bool  cb_keep_none(void *e, void *a);
NO_COV bool  cb_del_count(void *e, void *a);
NO_COV void *cb_merger(void *elem, void *arg);
NO_COV void *cb_map_identity(const void *elem);
NO_COV void *cb_map_fail(const void *elem);

void test_cm_hp_create(void);
void test_cm_hp_init(void);

void test_cm_hp_new_from_array(void);
void test_cm_hp_init_from_array(void);
void test_cm_hp_min_new_from_array(void);
void test_cm_hp_max_new_from_array(void);
void test_cm_hp_min_init_from_array(void);
void test_cm_hp_max_init_from_array(void);

void test_cm_hp_dup(void);
void test_cm_hp_ndup(void);

void test_cm_hp_clear(void);
void test_cm_hp_delete(void);
void test_cm_hp_free(void);

void test_cm_hp_is_empty(void);
void test_cm_hp_is_full(void);
void test_cm_hp_is_valid(void);
void test_cm_hp_min_is_valid(void);
void test_cm_hp_max_is_valid(void);

void test_cm_hp_min_heapify(void);
void test_cm_hp_max_heapify(void);

void test_cm_hp_get_cmp(void);
void test_cm_hp_get_cap(void);
void test_cm_hp_get_current_size(void);
void test_cm_hp_get_elem_size(void);
void test_cm_hp_get_elems_array(void);
void test_cm_hp_at(void);

void test_cm_hp_min_push(void);
void test_cm_hp_max_push(void);

void test_cm_hp_min_pop(void);
void test_cm_hp_max_pop(void);

void test_cm_hp_max_rm(void);
void test_cm_hp_min_rm(void);

void test_cm_hp_min_pushpop(void);
void test_cm_hp_max_pushpop(void);
void test_cm_hp_min_replace(void);
void test_cm_hp_max_replace(void);

void test_cm_hp_foreach(void);
void test_cm_hp_foreach_it(void);
void test_cm_hp_foreach_in(void);
void test_cm_hp_foreach_pre(void);
void test_cm_hp_foreach_post(void);

void test_cm_hp_peek(void);
void test_cm_hp_find(void);
void test_cm_hp_find_get(void);
void test_cm_hp_fold(void);
void test_cm_hp_reduce(void);

void test_cm_hp_min_filter(void);
void test_cm_hp_max_filter(void);

void test_cm_hp_map(void);

void test_cm_hp_iter(void);
void test_cm_hp_iter_rev(void);
void test_cm_hp_it_advance(void);
void test_cm_hp_it_back(void);
