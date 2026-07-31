#pragma once

// INFO: has to come BEFORE the fast_tls
struct s_mine {
	int value_a;
	int value_b;
};

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif

#define C_MINI__CTLS_INTERNAL
#define C_MINI__FTLS_INTERNAL
#define C_MINI__VTLS_INTERNAL

#define C_MINI__FTLS_STRUCT struct s_mine
#include "tls/common_thread_local_storage.h"
#include "tls/fast_thread_local_storage.h"
#include "tls/voided_thread_local_storage.h"

void  dump_mine_node(struct s_mine a);
void *ctls_dummy(void *__arg);
void *ftls_dummy(void *__arg);

// +----+
// |CTLS|---------<ctls>
// +----+

void test_ctls_rm(void);
void test_ctls_has(void);
void test_ctls_set(void);
void test_ctls_get(void);
void test_ctls_len(void);
void test_ctls_dump_typewrap(void);
void test_ctls_separate(void);

// +----+
// |FTLS|---------<ftls>
// +----+

void test_ftls_get(void);
void test_ftls_separate(void);

// +----+
// |VTLS|---------<vtls>
// +----+

void test_vtls_rm(void);
void test_vtls_get(void);
void test_vtls_len(void);
void test_vtls_has(void);
void test_vtls_set(void);
