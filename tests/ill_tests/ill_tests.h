#pragma once

#include <assert.h>

#include "../../ill/idl/intrusive_doubly_linked.h"
#include "../../ill/ill/intrusive_linked_lists.h"

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

// --- Main runners ---
void ill_main(void);
void idl_main(void);

// --- ILL tests ---
void test_ill_apply(void);
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
void test_idl_apply(void);
void test_idl_find(void);
void test_idl_get(void);
void test_idl_gettail(void);
void test_idl_init(void);
void test_idl_insert(void);
void test_idl_is(void);
void test_idl_length(void);
void test_idl_ntoe(void);
void test_idl_reverse(void);
void test_idl_rm(void);
