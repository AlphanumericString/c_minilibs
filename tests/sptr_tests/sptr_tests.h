#pragma once

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif

#include "sptr/smart_pointer.h"

void test_sptr_make_sharedv(void);
void test_sptr_make_sharedsptr(void);
void test_sptr_make_uniqv(void);
void test_sptr_make_uniqsptr(void);

void test_sptr_delete_shared(void);
void test_sptr_delete_uniq(void);

void test_sptr_strengthen(void);
void test_sptr_weaken(void);
void test_sptr_steal(void);

void test_sptr_copy(void);
void test_sptr_weak_copy(void);
void test_sptr_strong_copy(void);

void test_sptr_getdata(void);
void test_sptr_get_wc(void);
void test_sptr_get_sc(void);
void test_sptr_get_free_er(void);
