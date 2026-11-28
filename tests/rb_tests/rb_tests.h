#pragma once

#include <sys/mman.h>
#include <unistd.h>

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "common_base/tests.h"
#include "rb/round_buffer.h"

/* magic buffers round their byte capacity up to a whole page */
#define RB_TEST_PAGE_SIZE ((size_t)sysconf(_SC_PAGESIZE))
#define RB_TEST_ROUND_BYTES(nb_bytes)                                          \
	((((nb_bytes) + RB_TEST_PAGE_SIZE - 1) / RB_TEST_PAGE_SIZE) *              \
	 RB_TEST_PAGE_SIZE)
#define RB_TEST_PAGE_CAP(sz, n) (RB_TEST_ROUND_BYTES((sz) * (n)) / (sz))

#define ASSERT_RB_STATE(rb, sz, max, i, o)                                     \
	do {                                                                       \
		assert((rb).sz_e == (sz));                                             \
		assert((rb).nb_max == (max));                                          \
		assert((rb).i_pos == (i));                                             \
		assert((rb).o_pos == (o));                                             \
		assert((rb).array != NULL);                                            \
	} while (0);

bool  cb_true(void *elem, void *arg);
bool  cb_false(void *elem, void *arg);
bool  cb_add_one(void *elem, void *arg);
bool  cb_dump_int(void *elem, void *arg);
int   cb_cmp(const void *a, const void *b);
void *cb_dup(const void *elem);
void *cb_dup_fail3(const void *elem);
void  cb_destroy(void *elem);

void test_cm_rb_create(void);
void test_cm_rb_create_magic(void);
void test_cm_rb_init(void);
void test_cm_rb_from_array(void);
void test_cm_rb_from_array_owned(void);
void test_cm_rb_new(void);

void test_cm_rb_free(void);
void test_cm_rb_destroy(void);
void test_cm_rb_clr(void);

void test_cm_rb_full(void);
void test_cm_rb_empty(void);
void test_cm_rb_size(void);
void test_cm_rb_capacity(void);
void test_cm_rb_remaining_space(void);

void test_cm_rb_drop(void);
void test_cm_rb_pop(void);
void test_cm_rb_npop(void);
void test_cm_rb_peek(void);
void test_cm_rb_npeek(void);

void test_cm_rb_push(void);
void test_cm_rb_npush(void);
void test_cm_rb_push_safe(void);
void test_cm_rb_npush_safe(void);

void test_cm_rb_apply(void);
void test_cm_rb_naplly(void);

void test_cm_rb_filter(void);

void test_cm_rb_at(void);
void test_cm_rb_nat(void);

void test_cm_rb_get(void);
void test_cm_rb_nget(void);
void test_cm_rb_getn(void);

void test_cm_rb_find(void);

void test_cm_rb_map(void);
void test_cm_rb_nmap(void);

void test_cm_rb_grow(void);

void test_cm_rb_is_magic(void);
void test_cm_rb_magic_alias(void);
void test_cm_rb_magic_copy(void);
void test_cm_rb_magic_bulk(void);
void test_cm_rb_from_array_paths(void);
void test_cm_rb_mirror_helpers(void);
