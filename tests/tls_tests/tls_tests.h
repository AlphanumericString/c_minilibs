#pragma once

// has to come BEFORE the fast_tls
struct s_mine {
	int value_a;
	int value_b;
};

#define C_MINI__FTLS_STRUCT struct s_mine

#include "tls/common_thread_local_storage.h"
#include "tls/fast_thread_local_storage.h"
#include "tls/voided_thread_local_storage.h"

// print
void  dump_mine_node(struct s_mine a);
// tries to get toto then increment and print.
void *ctls_dummy(void *__arg);
// tries to get ftls set random(0-9) lst_status if lst_status == 0, error+=1
void *ftls_dummy(void *__arg);

// +----+
// |CTLS|---------<ctls>
// +----+
// remove
void test_ctls_rm(void);
// querry
void test_ctls_has(void);
// update / add
void test_ctls_set(void);
// get
void test_ctls_get(void);
void  test_ctls_len(void);
// utils
void test_ctls_dump_typewrap(void);

// thread spearation check
void	test_ctls_separate(void);

// +----+
// |FTLS|---------<ftls>
// +----+

void	test_ftls_get(void);
void	test_ftls_separate(void);

// +----+
// |VTLS|---------<vtls>
// +----+

// remove
void test_vtls_rm(void);

// get
void test_vtls_get(void);
void test_vtls_len(void);

// querry
void test_vtls_has(void);

// update / add
void test_vtls_set(void);
