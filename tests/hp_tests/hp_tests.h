#pragma once

#define C_MINI__TESTS
#include "hp/heap.h"

// create
void test_hp_create(void);
void test_hp_init(void);

// delete
void test_hp_clear(void);
void test_hp_delete(void);
void test_hp_free(void);

// push
void test_hp_min_push(void);
void test_hp_max_push(void);

// pop
void test_hp_min_pop(void);
void test_hp_max_pop(void);

// rm
void test_hp_max_rm(void);
void test_hp_min_rm(void);

// foreach
void test_hp_foreach(void);
void test_hp_foreach_in(void);
void test_hp_foreach_pre(void);
void test_hp_foreach_post(void);
