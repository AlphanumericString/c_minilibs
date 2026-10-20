#pragma once

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "common_base/tests.h"
#include "hp/heap.h"

void test_hp_create(void);
void test_hp_init(void);

void test_hp_clear(void);
void test_hp_delete(void);
void test_hp_free(void);

void test_hp_min_push(void);
void test_hp_max_push(void);

void test_hp_min_pop(void);
void test_hp_max_pop(void);

void test_hp_max_rm(void);
void test_hp_min_rm(void);

void test_hp_foreach(void);
void test_hp_foreach_in(void);
void test_hp_foreach_pre(void);
void test_hp_foreach_post(void);
