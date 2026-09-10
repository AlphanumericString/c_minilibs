#pragma once

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif

#include "sptr/smart_pointer.h"

// [make]
// make_shared
void test_sptr_make_shared_from_raw(void);
void test_sptr_make_shared_from_weak(void);
void test_sptr_make_shared_from_strong(void);
void test_sptr_make_shared_from_uniq(void);
// make_uniq
void test_sptr_make_uniq_from_raw(void);
void test_sptr_make_uniq_from_strong(void);
void test_sptr_make_uniq_from_uniq(void);

// [is]
// is_uniq
void test_sptr_is_uniqs(void);
void test_sptr_is_uniqw(void);
void test_sptr_is_uniqu(void);
// is_shared
void test_sptr_is_shareds(void);
void test_sptr_is_sharedw(void);
void test_sptr_is_sharedu(void);
// is_own
void test_sptr_is_owns(void);
void test_sptr_is_ownw(void);
void test_sptr_is_ownu(void);

// [delete]
void test_sptr_deletes(void);
void test_sptr_deletew(void);
void test_sptr_deleteu(void);

// [get]
// const-data
void test_sptr_getsc(void);
void test_sptr_getwc(void);
// data
void test_sptr_gets(void);
void test_sptr_getw(void);
void test_sptr_getu(void);
// ref-count
void test_sptr_get_rcs(void);
void test_sptr_get_rcw(void);
void test_sptr_get_rcu(void);
// weak-count
void test_sptr_get_wcs(void);
void test_sptr_get_wcw(void);
void test_sptr_get_wcu(void);

// [copy]
void test_sptr_copys(void);
void test_sptr_copyw(void);
void test_sptr_copyu(void);

// [dump]
void test_sptr_dumps(void);
void test_sptr_dumpw(void);
void test_sptr_dumpu(void);

// [change]
void test_sptr_strengthen(void);
void test_sptr_weaken(void);
void test_sptr_steal(void);
