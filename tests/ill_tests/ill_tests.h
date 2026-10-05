#pragma once

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "ill/idl/intrusive_doubly_linked.h"
#include "ill/ill/intrusive_linked_lists.h"

// --- Structs for testing ---
typedef struct s_test_node {
	int   id;
	int   value;
	t_ill node;
} t_test_node;

typedef struct s_test_dnode {
	int   id;
	int   value;
	t_idl dnode;
} t_test_dnode;

#define ASSERT_ORDER_IDL(node1, node2)                                         \
	assert((node1)->next == (node2) && (node2)->prev == (node1))

// --- ILL tests ---
void test_ill_foreach(void);
void test_ill_find(void);
void test_ill_get(void);
void test_ill_gettail(void);
void test_ill_init(void);
void test_ill_insert(void);
void test_ill_is(void);
void test_ill_length(void);
void test_ill_ntoe(void);
void test_ill_reverse(void);
void test_ill_rm(void);

// --- IDL tests ---
void test_idl_foreach(void);
void test_idl_find(void);
void test_idl_get(void);
void test_idl_gettail(void);
void test_idl_gethead(void);
void test_idl_init(void);
void test_idl_insert(void);
void test_idl_is(void);
void test_idl_length(void);
void test_idl_ntoe(void);
void test_idl_reverse(void);
void test_idl_rm(void);

void test_idl_append(void);
void test_idl_copy(void);
void test_idl_eton(void);
void test_idl_find_prev(void);
void test_idl_find(void);
void test_idl_foreach(void);
void test_idl_for(void);
void test_idl_get_kth(void);
void test_idl_get_max(void);
void test_idl_get_min(void);
void test_idl_get_prev(void);
void test_idl_get_tail(void);
void test_idl_get(void);
void test_idl_has(void);
void test_idl_init(void);
void test_idl_is_circular(void);
void test_idl_is_isolated(void);
void test_idl_is_sorted(void);
void test_idl_length(void);
void test_idl_match_nb(void);
void test_idl_nappend(void);
void test_idl_nlength(void);
void test_idl_nprepend(void);
void test_idl_npush(void);
void test_idl_ntoe(void);
void test_idl_pop_back(void);
void test_idl_pop(void);
void test_idl_prepend(void);
void test_idl_push_back(void);
void test_idl_push(void);
void test_idl_remove_after(void);
void test_idl_remove_node(void);
void test_idl_reverse(void);
