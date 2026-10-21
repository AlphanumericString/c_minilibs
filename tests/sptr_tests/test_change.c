#include <assert.h>

#include "sptr_tests.h"

static int  g_change_dtor_calls = 0;
static void change_dummy_dtor(void *p) {
	(void)p;
	g_change_dtor_calls++;
}
static void local_dtor(void *_) {
	return;
}
static void other_dtor(void *_) {
	return;
}

// strengthen (sptr_into_strong)
static void test_sptr_strengthen_errors(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_no_wc = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b1_no_rc = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};

	t_weak_ptr w0_no_wc     = (t_weak_ptr){&b0_no_wc};
	t_weak_ptr w1_no_rc     = (t_weak_ptr){&b1_no_rc};
	t_weak_ptr w2_meta_null = (t_weak_ptr){NULL};

	t_strong_ptr res[] = {
	    sptr_into_strong(NULL),
	    sptr_into_strong(&w0_no_wc),
	    sptr_into_strong(&w1_no_rc),
	    sptr_into_strong(&w2_meta_null),
	};

	assert(!res[0].data && !res[0].meta);
	assert(!res[1].data && !res[1].meta);
	assert(!res[2].data && !res[2].meta);
	assert(!res[3].data && !res[3].meta);
}

static void test_sptr_strengthen_ok(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b1 = (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 3};

	t_weak_ptr w0 = (t_weak_ptr){&b0};
	t_weak_ptr w1 = (t_weak_ptr){&b1};

	t_strong_ptr res[] = {
	    sptr_into_strong(&w0),
	    sptr_into_strong(&w1),
	};

	assert(res[0].data == data && res[0].meta == &b0);
	assert(res[1].data == data && res[1].meta == &b1);

	// rc incremented, wc decremented
	assert(b0.rc == 2 && b0.wc == 0);
	assert(b1.rc == 3 && b1.wc == 2);
}

NO_COV void test_sptr_strengthen(void) {
	test_sptr_strengthen_errors();
	test_sptr_strengthen_ok();
}

// weaken (sptr_into_weak)
static void test_sptr_weaken_errors(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_no_rc = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};

	t_strong_ptr s0_no_rc     = (t_strong_ptr){b0_no_rc.data, &b0_no_rc};
	t_strong_ptr s1_meta_null = (t_strong_ptr){data, NULL};

	t_weak_ptr res[] = {
	    sptr_into_weak(NULL),
	    sptr_into_weak(&s0_no_rc),
	    sptr_into_weak(&s1_meta_null),
	};

	assert(!res[0].meta);
	assert(!res[1].meta);
	assert(!res[2].meta);
}

static void test_sptr_weaken_ok(void) {
	void *data = (void *)0xDEAD;

	t_fbox b0_rc2 =
	    (t_fbox){.data = data, .dtor = change_dummy_dtor, .rc = 2, .wc = 0};
	t_fbox b1_rc1 =
	    (t_fbox){.data = data, .dtor = change_dummy_dtor, .rc = 1, .wc = 1};
	t_fbox b2_no_dt = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};

	t_strong_ptr s0 = (t_strong_ptr){b0_rc2.data, &b0_rc2};
	t_strong_ptr s1 = (t_strong_ptr){b1_rc1.data, &b1_rc1};
	t_strong_ptr s2 = (t_strong_ptr){b2_no_dt.data, &b2_no_dt};

	g_change_dtor_calls = 0;

	t_weak_ptr res[] = {
	    sptr_into_weak(&s0),
	    sptr_into_weak(&s1),
	    sptr_into_weak(&s2),
	};

	assert(res[0].meta == &b0_rc2);
	assert(res[1].meta == &b1_rc1);
	assert(res[2].meta == &b2_no_dt);

	// s0: rc decremented to 1, wc incremented to 1, dtor not called
	assert(b0_rc2.rc == 1 && b0_rc2.wc == 1);
	assert(b0_rc2.data == data);

	// s1: rc decremented to 0, wc incremented to 2, dtor called, data cleared
	assert(b1_rc1.rc == 0 && b1_rc1.wc == 2);
	assert(b1_rc1.data == NULL);
	assert(g_change_dtor_calls == 1);

	// s2: rc decremented to 0, wc incremented to 2, null dtor called, data
	// cleared
	assert(b2_no_dt.rc == 0 && b2_no_dt.wc == 2);
	assert(!b2_no_dt.data);
}

NO_COV void test_sptr_weaken(void) {
	test_sptr_weaken_errors();
	test_sptr_weaken_ok();
}

// transfer_ownership (sptr_transfer_ownership)
static void test_sptr_transfer_o_errors(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b1 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};

	t_owned_ptr o0_owner_b0  = (t_owned_ptr){b0.data, &b0, true};
	t_owned_ptr o1_borrow_b0 = (t_owned_ptr){b0.data, &b0, false};
	t_owned_ptr o2_owner2_b0 = (t_owned_ptr){b0.data, &b0, true};
	t_owned_ptr o3_borrow_b1 = (t_owned_ptr){b1.data, &b1, false};
	t_owned_ptr o4_meta_null = (t_owned_ptr){data, NULL, true};

	bool res[] = {
	    sptr_transfer_ownership(NULL, NULL),
	    sptr_transfer_ownership(&o1_borrow_b0, NULL),
	    sptr_transfer_ownership(NULL, &o0_owner_b0),
	    sptr_transfer_ownership(&o0_owner_b0, &o1_borrow_b0),  // to already own
	    sptr_transfer_ownership(&o1_borrow_b0, &o3_borrow_b1), // from !owner
	    sptr_transfer_ownership(&o2_owner2_b0, &o0_owner_b0),  // to&from == own
	    sptr_transfer_ownership(&o3_borrow_b1, &o0_owner_b0),  // boxes differ
	    sptr_transfer_ownership(&o4_meta_null, &o0_owner_b0),  // to meta null
	    sptr_transfer_ownership(&o1_borrow_b0, &o4_meta_null), // from meta null
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
	assert(!res[5]);
	assert(!res[6]);
	assert(!res[7]);
	assert(!res[8]);
}

static void test_sptr_transfer_o_ok(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};

	t_owned_ptr o_to   = (t_owned_ptr){b0.data, &b0, false};
	t_owned_ptr o_from = (t_owned_ptr){b0.data, &b0, true};

	bool res = sptr_transfer_ownership(&o_to, &o_from);

	assert(res == true);
	assert(o_to.owner == true);
	assert(!o_from.owner);
}

NO_COV void test_sptr_transfer_o(void) {
	test_sptr_transfer_o_errors();
	test_sptr_transfer_o_ok();
}

// move_ownership (sptr_move_ownership)
static void test_sptr_move_o_errors(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_not_owner =
	    (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b1_no_rc   = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_fbox b2_rc_many = (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 1};
	t_fbox b3_ok      = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};

	t_owned_ptr o0_dst = (t_owned_ptr){0};
	t_owned_ptr o1_not_owner =
	    (t_owned_ptr){b0_not_owner.data, &b0_not_owner, false};
	t_owned_ptr o2_no_rc   = (t_owned_ptr){b1_no_rc.data, &b1_no_rc, true};
	t_owned_ptr o3_rc_many = (t_owned_ptr){b2_rc_many.data, &b2_rc_many, true};
	t_owned_ptr o4_src_meta_nil = (t_owned_ptr){data, NULL, true};
	t_owned_ptr o5_dst_non_null = (t_owned_ptr){data, &b2_rc_many, true};
	t_owned_ptr o6_ok           = (t_owned_ptr){data, &b3_ok, true};

	bool res[] = {
	    sptr_move_ownership(NULL, NULL),
	    sptr_move_ownership(&o0_dst, NULL),
	    sptr_move_ownership(NULL, &o1_not_owner),
	    sptr_move_ownership(&o0_dst, &o1_not_owner),
	    sptr_move_ownership(&o0_dst, &o2_no_rc),
	    sptr_move_ownership(&o0_dst, &o3_rc_many),
	    sptr_move_ownership(&o0_dst, &o4_src_meta_nil),
	    sptr_move_ownership(&o5_dst_non_null, &o6_ok),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
	assert(!res[5]);
	assert(!res[6]);
	assert(!res[7]);
}

static void test_sptr_move_o_ok(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	// Case 1: to has meta == NULL
	t_fbox      b0    = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_owned_ptr to0   = (t_owned_ptr){0};
	t_owned_ptr from0 = (t_owned_ptr){b0.data, &b0, true};

	bool res0 = sptr_move_ownership(&to0, &from0);

	assert(res0 == true);
	assert(to0.owner == true);
	assert(to0.meta == &b0 && to0.data == data);
	assert(from0.owner == false);
	// wc incremented by sptr_copyo inside sptr_move_ownership
	assert(b0.wc == 0);

	// Case 2: to has an existing box with wc > 0 (so sptr_deleteo doesn't free
	// stack)
	t_fbox b_old =
	    (t_fbox){.data = (void *)0x1234, .dtor = NULL, .rc = 1, .wc = 1};
	t_fbox b1 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};

	t_owned_ptr to1   = (t_owned_ptr){b_old.data, &b_old, true};
	t_owned_ptr from1 = (t_owned_ptr){b1.data, &b1, true};

	bool res1 = sptr_move_ownership(&to1, &from1);

	assert(res1 == true);
	assert(to1.owner == true);
	assert(to1.meta == &b1 && to1.data == data);
	assert(from1.owner == false);
	assert(b1.wc == 0);
	// b_old was deleted by sptr_deleteo(to): its rc was decremented to 0
	assert(b_old.rc == 0);
}

NO_COV void test_sptr_move_o(void) {
	test_sptr_move_o_errors();
	test_sptr_move_o_ok();
}

// yield (sptr_uniq_yield)
static void test_sptr_yield_errors(void) {
	void      *data = (void *)0xDEAD;
	t_uniq_ptr u    = (t_uniq_ptr){.data = data, .dtor = NULL};

	t_uniq_ptr *res[] = {
	    sptr_uniq_yield(NULL, NULL),
	    sptr_uniq_yield(&u, NULL),
	    sptr_uniq_yield(NULL, &u),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
}

static void test_sptr_yield_ok(void) {
	void *data1 = (void *)0xDEAD;
	void *data2 = (void *)0xBEEF;

	// Case 1: dst has data (should be deleted / dtor called)
	t_uniq_ptr dst1 = (t_uniq_ptr){.data = data1, .dtor = change_dummy_dtor};
	t_uniq_ptr src1 = (t_uniq_ptr){.data = data2, .dtor = NULL};

	g_change_dtor_calls = 0;
	t_uniq_ptr *r1      = sptr_uniq_yield(&dst1, &src1);

	assert(r1 == &dst1);
	assert(dst1.data == data2 && !dst1.dtor);
	assert(!src1.data && !src1.dtor);
	assert(g_change_dtor_calls == 1);

	// Case 2: dst has no data
	t_uniq_ptr dst2 = (t_uniq_ptr){0};
	t_uniq_ptr src2 = (t_uniq_ptr){.data = data1, .dtor = local_dtor};

	t_uniq_ptr *r2 = sptr_uniq_yield(&dst2, &src2);

	assert(r2 == &dst2);
	assert(dst2.data == data1 && dst2.dtor == local_dtor);
	assert(!src2.data && !src2.dtor);
}

NO_COV void test_sptr_yield(void) {
	test_sptr_yield_errors();
	test_sptr_yield_ok();
}

// swap (sptr_uniq_swap)
static void test_sptr_swap_errors(void) {
	void      *data = (void *)0xDEAD;
	t_uniq_ptr u    = (t_uniq_ptr){.data = data, .dtor = NULL};

	bool res[] = {
	    sptr_uniq_swap(NULL, NULL),
	    sptr_uniq_swap(&u, NULL),
	    sptr_uniq_swap(NULL, &u),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
}

static void test_sptr_swap_ok(void) {
	void  *data1 = (void *)0xDEAD;
	void  *data2 = (void *)0xBEEF;
	t_dtor dtor1 = local_dtor;
	t_dtor dtor2 = other_dtor;

	t_uniq_ptr sw1 = (t_uniq_ptr){.data = data1, .dtor = dtor1};
	t_uniq_ptr sw2 = (t_uniq_ptr){.data = data2, .dtor = dtor2};

	bool res = sptr_uniq_swap(&sw1, &sw2);

	assert(res == true);
	assert(sw1.data == data2 && sw1.dtor == dtor2);
	assert(sw2.data == data1 && sw2.dtor == dtor1);
}

NO_COV void test_sptr_swap(void) {
	test_sptr_swap_errors();
	test_sptr_swap_ok();
}
