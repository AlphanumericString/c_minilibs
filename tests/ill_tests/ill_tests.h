#pragma once

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "common_base/tests.h"
#include "ill/idl/intrusive_doubly_linked.h"
#include "ill/ill/intrusive_linked_lists.h"
#include "ill/lh/list_handler.h"

// --- Structs for testing ---
typedef struct s_cm_test_node {
	int      id;
	int      value;
	t_cm_ill node;
} t_cm_test_node;

typedef struct s_cm_test_dnode {
	int      id;
	int      value;
	t_cm_idl dnode;
} t_cm_test_dnode;

typedef struct s_cm_it_state {
	size_t count;
	size_t first_pos;
	size_t last_pos;
	size_t nb_last;
	long   sum;
} t_cm_it_state;

#define ASSERT_ORDER_IDL(node1, node2)                                         \
	assert((node1)->next == (node2) && (node2)->prev == (node1))

#define INIT_DNODE(n, _val, _id)                                               \
	idl_init_node(&((n).dnode), offsetof(t_cm_test_dnode, dnode));             \
	(n).value = (_val);                                                        \
	(n).id    = (_id);

#define INIT_STD_DNODES(n1, n2, n3, n4)                                        \
	INIT_DNODE((n1), 42, 1);                                                   \
	INIT_DNODE((n2), 43, 2);                                                   \
	INIT_DNODE((n3), 44, 3);                                                   \
	INIT_DNODE((n4), 45, 4);

#define LINK_UP_DNODES(n1, n2)                                                 \
	(n1)->next = (n2);                                                         \
	(n2)->prev = (n1);

#define ASSERT_ORDER_ILL(node1, node2) assert((node1)->next == (node2))

#define INIT_NODE(n, _val, _id)                                                \
	ill_init_node(&((n).node), offsetof(t_cm_test_node, node));                \
	(n).value = (_val);                                                        \
	(n).id    = (_id);

#define INIT_STD_NODES(n1, n2, n3, n4)                                         \
	INIT_NODE((n1), 42, 1);                                                    \
	INIT_NODE((n2), 43, 2);                                                    \
	INIT_NODE((n3), 44, 3);                                                    \
	INIT_NODE((n4), 45, 4);

#define LINK_UP_NODES(n1, n2) (n1)->next = (n2);

// --- Callbacks ---
// --general callbacks
NO_COV bool  cb_true(void *elem, void *arg);
NO_COV bool  cb_false(void *elem, void *arg);
NO_COV void *cb_ill_sum(void *elem, void *arg);
NO_COV void *cb_idl_sum(void *elem, void *arg);
NO_COV void *cb_bad_duplicator(const void *elem);
NO_COV void  cb_free_elem(void *elem);
NO_COV void  cb_count_dtor(void *elem);
NO_COV void *cb_ill_duplicate_fail_at(const void *elem);
NO_COV void *cb_idl_duplicate_fail_at(const void *elem);
extern int   g_dtor_hits;
extern int   g_dup_hits;
extern int   g_dup_fail_at;
// --idl only callbacks
NO_COV bool  cb_idl_add_val_as_int_ptr(void *elem, void *arg);
NO_COV bool  cb_idl_it_add_pos(void *elem, void *arg, size_t pos, bool last);
NO_COV bool  cb_idl_it_record(void *elem, void *arg, size_t pos, bool last);
NO_COV bool  cb_idl_it_stop(void *elem, void *arg, size_t pos, bool last);
NO_COV bool  cb_idl_false_on_arg(void *elem, void *arg);
NO_COV bool  cb_idl_has_val_42(void *elem, void *arg);
NO_COV bool  cb_idl_is_even(void *elem, void *arg);
NO_COV int   cb_idl_cmp_val(const void *elem, const void *arg);
NO_COV int   cb_idl_cmp_min_max(const void *elem1, const void *elem2);
NO_COV void *cb_idl_duplicate(const void *elem);
// --ill only callbacks
NO_COV bool  cb_ill_add_val_as_int_ptr(void *elem, void *arg);
NO_COV bool  cb_ill_it_add_pos(void *elem, void *arg, size_t pos, bool last);
NO_COV bool  cb_ill_it_record(void *elem, void *arg, size_t pos, bool last);
NO_COV bool  cb_ill_it_stop(void *elem, void *arg, size_t pos, bool last);
NO_COV bool  cb_ill_stop_at_30(void *elem, void *arg);
NO_COV bool  cb_ill_has_val_42(void *elem, void *arg);
NO_COV bool  cb_ill_is_even(void *elem, void *arg);
NO_COV int   cb_ill_cmp_val(const void *elem, const void *arg);
NO_COV int   cb_ill_cmp_min_max(const void *elem1, const void *elem2);
NO_COV void *cb_ill_duplicate(const void *elem);

// --- ILL tests ---
NO_COV void test_cm_ill_advance_it(void);
NO_COV void test_cm_ill_append(void);
NO_COV void test_cm_ill_append_node(void);
NO_COV void test_cm_ill_copy(void);
NO_COV void test_cm_ill_eton(void);
NO_COV void test_cm_ill_find(void);
NO_COV void test_cm_ill_find_node(void);
NO_COV void test_cm_ill_find_prev(void);
NO_COV void test_cm_ill_find_prev_node(void);
NO_COV void test_cm_ill_filter(void);
NO_COV void test_cm_ill_filter_out(void);
NO_COV void test_cm_ill_iter(void);
NO_COV void test_cm_ill_napply(void);
NO_COV void test_cm_ill_apply(void);
NO_COV void test_cm_ill_get(void);
NO_COV void test_cm_ill_get_node(void);
NO_COV void test_cm_ill_get_kth(void);
NO_COV void test_cm_ill_get_kth_node(void);
NO_COV void test_cm_ill_get_max(void);
NO_COV void test_cm_ill_get_max_node(void);
NO_COV void test_cm_ill_get_min(void);
NO_COV void test_cm_ill_get_min_node(void);
NO_COV void test_cm_ill_get_prev(void);
NO_COV void test_cm_ill_get_prev_node(void);
NO_COV void test_cm_ill_get_tail(void);
NO_COV void test_cm_ill_get_tail_node(void);
NO_COV void test_cm_ill_has(void);
NO_COV void test_cm_ill_init(void);
NO_COV void test_cm_ill_init_node(void);
NO_COV void test_cm_ill_is_circular(void);
NO_COV void test_cm_ill_is_isolated(void);
NO_COV void test_cm_ill_is_isolated_node(void);
NO_COV void test_cm_ill_is_sorted(void);
NO_COV void test_cm_ill_length(void);
NO_COV void test_cm_ill_match_nb(void);
NO_COV void test_cm_ill_nappend(void);
NO_COV void test_cm_ill_nappend_node(void);
NO_COV void test_cm_ill_nlength(void);
NO_COV void test_cm_ill_npush(void);
NO_COV void test_cm_ill_npush_node(void);
NO_COV void test_cm_ill_ntoe(void);
NO_COV void test_cm_ill_pop(void);
NO_COV void test_cm_ill_pop_node(void);
NO_COV void test_cm_ill_pop_back(void);
NO_COV void test_cm_ill_pop_back_node(void);
NO_COV void test_cm_ill_push(void);
NO_COV void test_cm_ill_push_node(void);
NO_COV void test_cm_ill_push_back(void);
NO_COV void test_cm_ill_push_back_node(void);
NO_COV void test_cm_ill_remove(void);
NO_COV void test_cm_ill_remove_after(void);
NO_COV void test_cm_ill_remove_after_node(void);
NO_COV void test_cm_ill_remove_node(void);
NO_COV void test_cm_ill_reverse(void);
NO_COV void test_cm_ill_filter_dt(void);
NO_COV void test_cm_ill_sort(void);
NO_COV void test_cm_ill_unique(void);

// --- ILL additions ---
NO_COV void test_cm_ill_apply_it(void);
NO_COV void test_cm_ill_napply_it(void);
NO_COV void test_cm_ill_apply_range(void);
NO_COV void test_cm_ill_clear(void);
NO_COV void test_cm_ill_concat(void);
NO_COV void test_cm_ill_fold(void);
NO_COV void test_cm_ill_is_empty(void);
NO_COV void test_cm_ill_iter_range(void);
NO_COV void test_cm_ill_move(void);
NO_COV void test_cm_ill_nfilter(void);
NO_COV void test_cm_ill_validate(void);
NO_COV void test_cm_ill_internal(void);

// --- IDL tests ---
NO_COV void test_cm_idl_advance_it(void);
NO_COV void test_cm_idl_append(void);
NO_COV void test_cm_idl_append_node(void);
NO_COV void test_cm_idl_copy(void);
NO_COV void test_cm_idl_eton(void);
NO_COV void test_cm_idl_find(void);
NO_COV void test_cm_idl_find_node(void);
NO_COV void test_cm_idl_find_prev(void);
NO_COV void test_cm_idl_find_prev_node(void);
NO_COV void test_cm_idl_filter(void);
NO_COV void test_cm_idl_filter_out(void);
NO_COV void test_cm_idl_iter(void);
NO_COV void test_cm_idl_napply(void);
NO_COV void test_cm_idl_apply(void);
NO_COV void test_cm_idl_get(void);
NO_COV void test_cm_idl_get_node(void);
NO_COV void test_cm_idl_get_head(void);
NO_COV void test_cm_idl_get_head_node(void);
NO_COV void test_cm_idl_get_kth(void);
NO_COV void test_cm_idl_get_kth_node(void);
NO_COV void test_cm_idl_get_max(void);
NO_COV void test_cm_idl_get_max_node(void);
NO_COV void test_cm_idl_get_min(void);
NO_COV void test_cm_idl_get_min_node(void);
NO_COV void test_cm_idl_get_prev(void);
NO_COV void test_cm_idl_get_prev_node(void);
NO_COV void test_cm_idl_get_tail(void);
NO_COV void test_cm_idl_get_tail_node(void);
NO_COV void test_cm_idl_has(void);
NO_COV void test_cm_idl_init(void);
NO_COV void test_cm_idl_init_node(void);
NO_COV void test_cm_idl_is_circular(void);
NO_COV void test_cm_idl_is_isolated(void);
NO_COV void test_cm_idl_is_isolated_node(void);
NO_COV void test_cm_idl_is_sorted(void);
NO_COV void test_cm_idl_length(void);
NO_COV void test_cm_idl_match_nb(void);
NO_COV void test_cm_idl_nappend(void);
NO_COV void test_cm_idl_nappend_node(void);
NO_COV void test_cm_idl_nlength(void);
NO_COV void test_cm_idl_nprepend(void);
NO_COV void test_cm_idl_nprepend_node(void);
NO_COV void test_cm_idl_npush(void);
NO_COV void test_cm_idl_npush_node(void);
NO_COV void test_cm_idl_ntoe(void);
NO_COV void test_cm_idl_pop(void);
NO_COV void test_cm_idl_pop_node(void);
NO_COV void test_cm_idl_pop_back(void);
NO_COV void test_cm_idl_pop_back_node(void);
NO_COV void test_cm_idl_prepend(void);
NO_COV void test_cm_idl_prepend_node(void);
NO_COV void test_cm_idl_push(void);
NO_COV void test_cm_idl_push_node(void);
NO_COV void test_cm_idl_push_back(void);
NO_COV void test_cm_idl_push_back_node(void);
NO_COV void test_cm_idl_remove(void);
NO_COV void test_cm_idl_remove_after(void);
NO_COV void test_cm_idl_remove_after_node(void);
NO_COV void test_cm_idl_remove_node(void);
NO_COV void test_cm_idl_reverse(void);
NO_COV void test_cm_idl_filter_dt(void);
NO_COV void test_cm_idl_sort(void);
NO_COV void test_cm_idl_unique(void);

// --- ILL circular ---
NO_COV void test_cm_ill_circ_init(void);
NO_COV void test_cm_ill_circ_push_pop(void);
NO_COV void test_cm_ill_circ_concat(void);
NO_COV void test_cm_ill_circ_remove_clear(void);

// --- IDL circular ---
NO_COV void test_cm_idl_circ_init(void);
NO_COV void test_cm_idl_circ_push_pop(void);
NO_COV void test_cm_idl_circ_concat(void);
NO_COV void test_cm_idl_circ_remove_clear(void);

// --- LH handler ---
NO_COV void test_cm_lh_create(void);
NO_COV void test_cm_lh_singly(void);
NO_COV void test_cm_lh_doubly(void);
NO_COV void test_cm_lh_circ_singly(void);
NO_COV void test_cm_lh_circ_doubly(void);
NO_COV void test_cm_lh_invalid_type(void);
NO_COV void test_cm_lh_edges(void);
NO_COV void test_cm_lh_concat(void);

// --- IDL additions ---
NO_COV void test_cm_idl_apply_it(void);
NO_COV void test_cm_idl_napply_it(void);
NO_COV void test_cm_idl_apply_range(void);
NO_COV void test_cm_idl_clear(void);
NO_COV void test_cm_idl_concat(void);
NO_COV void test_cm_idl_fold(void);
NO_COV void test_cm_idl_is_empty(void);
NO_COV void test_cm_idl_iter_range(void);
NO_COV void test_cm_idl_iter_rev(void);
NO_COV void test_cm_idl_move(void);
NO_COV void test_cm_idl_nfilter(void);
NO_COV void test_cm_idl_validate(void);
NO_COV void test_cm_idl_internal(void);
