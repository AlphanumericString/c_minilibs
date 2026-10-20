#pragma once

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "common_base/tests.h"
#include "rb/round_buffer.h"

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

void test_rb_create(void);
void test_rb_init(void);
void test_rb_from_array(void);
void test_rb_new(void);

void test_rb_free(void);
void test_rb_destroy(void);
void test_rb_clr(void);

void test_rb_full(void);
void test_rb_empty(void);
void test_rb_size(void);
void test_rb_capacity(void);
void test_rb_remaining_space(void);

void test_rb_drop(void);
void test_rb_pop(void);
void test_rb_npop(void);
void test_rb_peek(void);
void test_rb_npeek(void);

void test_rb_push(void);
void test_rb_npush(void);
void test_rb_push_safe(void);
void test_rb_npush_safe(void);

void test_rb_apply(void);
void test_rb_naplly(void);

void test_rb_filter(void);

void test_rb_at(void);
void test_rb_nat(void);

void test_rb_get(void);
void test_rb_nget(void);

void test_rb_find(void);

void test_rb_map(void);
void test_rb_nmap(void);

void test_rb_grow(void);
