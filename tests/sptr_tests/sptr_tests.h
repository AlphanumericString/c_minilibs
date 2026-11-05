#pragma once

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif

#define C_MINI__SPTR_INTERNAL
#include "common_base/tests.h"
#include "sptr/smart_pointer.h"

// [make]
// make_shared
void test_sptr_make_shared_from_raw(void);
void test_sptr_make_shared_from_weak(void);
void test_sptr_make_shared_from_strong(void);
void test_sptr_make_shared_from_owned(void);
void test_sptr_make_shared_from_uniq(void);
// make_weak
void test_sptr_make_weak_from_weak(void);
void test_sptr_make_weak_from_strong(void);
// make_owned
void test_sptr_make_owned_from_raw(void);
void test_sptr_make_owned_from_strong(void);
void test_sptr_make_owned_from_owned(void);
void test_sptr_make_owned_from_uniq(void);
// make uniq
void test_sptr_make_uniq_from_raw(void);
void test_sptr_make_uniq_from_weak(void);
void test_sptr_make_uniq_from_strong(void);
void test_sptr_make_uniq_from_owned(void);
void test_sptr_make_uniq_from_uniq(void);

// [is]
// is_uniq
void test_sptr_is_uniqs(void);
void test_sptr_is_uniqw(void);
void test_sptr_is_uniqo(void);
void test_sptr_is_uniqu(void);
// is_shared
void test_sptr_is_shareds(void);
void test_sptr_is_sharedw(void);
void test_sptr_is_sharedo(void);
void test_sptr_is_sharedu(void);
// is_sole_box_owner
void test_sptr_is_sole_box_owns(void);
void test_sptr_is_sole_box_ownw(void);
void test_sptr_is_sole_box_owno(void);
void test_sptr_is_sole_box_ownu(void);

// [delete]
void test_sptr_deletes(void);
void test_sptr_deletew(void);
void test_sptr_deleteo(void);
void test_sptr_deleteu(void);

// [get]
// value by const-ptr
void test_sptr_getsc(void);
void test_sptr_getwc(void);
void test_sptr_getoc(void);
void test_sptr_getuc(void);
// value by non-const-ptr
void test_sptr_gets(void);
void test_sptr_getw(void);
void test_sptr_geto(void);
void test_sptr_getu(void);

// ref-count
void test_sptr_get_rcs(void);
void test_sptr_get_rcw(void);
void test_sptr_get_rco(void);
void test_sptr_get_rcu(void);
// weak-count
void test_sptr_get_wcs(void);
void test_sptr_get_wcw(void);
void test_sptr_get_wco(void);
void test_sptr_get_wcu(void);
// dtor
void test_sptr_get_dtors(void);
void test_sptr_get_dtorw(void);
void test_sptr_get_dtoro(void);
void test_sptr_get_dtoru(void);

// [set]
// dtor
void test_sptr_set_dtors(void);
void test_sptr_set_dtorw(void);
void test_sptr_set_dtoro(void);
void test_sptr_set_dtoru(void);

// [copy]
void test_sptr_copys(void);
void test_sptr_copyw(void);
void test_sptr_copyo(void);
void test_sptr_copyu(void);

// [dump]
void test_sptr_to_string_s(void);
void test_sptr_to_string_w(void);
void test_sptr_to_string_o(void);
void test_sptr_to_string_u(void);
void test_sptr_dumps(void);
void test_sptr_dumpw(void);
void test_sptr_dumpo(void);
void test_sptr_dumpu(void);

// [fbox]
void test_cm_fbox_get_count(void);
void test_cm_fbox_increment(void);
void test_cm_fbox_decrement(void);
void test_cm_fbox_get_cm_data(void);
void test_cm_fbox_init(void);
void test_cm_fbox_new(void);

// [change]
void test_sptr_strengthen(void);
void test_sptr_weaken(void);
void test_sptr_transfer_o(void);
void test_sptr_move_o(void);
void test_sptr_yield(void);
void test_sptr_swap(void);
