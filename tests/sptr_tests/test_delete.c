#include <assert.h>
#include <stdlib.h>

#define C_MINI__INTERNAL
#include "sptr_tests.h"

static int  g_dtor_calls = 0;
static void dummy_dt(void *p) {
	(void)p;
	g_dtor_calls++;
}

// deletes
static void test_sptr_deletes_errors(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_fbox b0_no_rc = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};

	t_strong_ptr s0_no_rc     = (t_strong_ptr){b0_no_rc.data, &b0_no_rc};
	t_strong_ptr s1_meta_null = (t_strong_ptr){data, NULL};

	bool res[] = {
	    sptr_deletes(NULL),
	    sptr_deletes(&s0_no_rc),
	    sptr_deletes(&s1_meta_null),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
}

static void test_sptr_deletes_ok(void) {
	void *data = (void *)0xDEAD;

	t_fbox  b0_rc2 = (t_fbox){.data = data, .dtor = dummy_dt, .rc = 2, .wc = 0};
	t_fbox  b1_wc1 = (t_fbox){.data = data, .dtor = dummy_dt, .rc = 1, .wc = 1};
	t_fbox *b2_rc_1    = malloc(sizeof(*b2_rc_1));
	t_fbox  b3_dt_none = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};

	assert(b2_rc_1 != NULL);
	*b2_rc_1 = (t_fbox){.data = data, .dtor = dummy_dt, .rc = 1, .wc = 0};

	t_strong_ptr s0_rc2   = (t_strong_ptr){b0_rc2.data, &b0_rc2};
	t_strong_ptr s1_wc1   = (t_strong_ptr){b1_wc1.data, &b1_wc1};
	t_strong_ptr s2_heap  = (t_strong_ptr){b2_rc_1->data, b2_rc_1};
	t_strong_ptr s3_no_dt = (t_strong_ptr){b3_dt_none.data, &b3_dt_none};

	g_dtor_calls = 0;
	assert(sptr_get_wc(&s1_wc1) == 1);

	bool res[] = {
	    sptr_deletes(&s0_rc2),
	    sptr_deletes(&s1_wc1),
	    sptr_deletes(&s2_heap),
	    sptr_deletes(&s3_no_dt),
	};

	assert(res[0] == true);
	assert(res[1] == true);
	assert(res[2] == true);
	assert(res[3] == true);

	// Pointers zeroed out
	assert(!s0_rc2.data && !s0_rc2.meta);
	assert(!s1_wc1.data && !s1_wc1.meta);
	assert(!s2_heap.data && !s2_heap.meta);
	assert(!s3_no_dt.data && !s3_no_dt.meta);

	// s0: rc decremented to 1, dtor not called
	assert(b0_rc2.rc == 1);

	// dtor called for s1 and s2 (total 2)
	assert(g_dtor_calls == 2);
}

void test_sptr_deletes(void) {
	test_sptr_deletes_errors();
	test_sptr_deletes_ok();
}

// deletew
static void test_sptr_deletew_errors(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_fbox b0_no_wc = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};

	t_weak_ptr w0_no_wc     = (t_weak_ptr){&b0_no_wc};
	t_weak_ptr w1_meta_null = (t_weak_ptr){NULL};

	bool res[] = {
	    sptr_deletew(NULL),
	    sptr_deletew(&w0_no_wc),
	    sptr_deletew(&w1_meta_null),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
}

static void test_sptr_deletew_ok(void) {
	void *data = (void *)0xDEAD;

	t_fbox b0_wc2 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 2};
	t_fbox b1_rc1 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};

	t_fbox *heap_box = malloc(sizeof(*heap_box));
	assert(heap_box != NULL);
	*heap_box = (t_fbox){.data = NULL, .dtor = NULL, .rc = 0, .wc = 1};

	t_weak_ptr w0_wc2  = (t_weak_ptr){&b0_wc2};
	t_weak_ptr w1_rc1  = (t_weak_ptr){&b1_rc1};
	t_weak_ptr w2_heap = (t_weak_ptr){heap_box};

	bool res[] = {
	    sptr_deletew(&w0_wc2),
	    sptr_deletew(&w1_rc1),
	    sptr_deletew(&w2_heap),
	};

	assert(res[0] == true);
	assert(res[1] == true);
	assert(res[2] == true);

	// Weak pointers zeroed out
	assert(!w0_wc2.meta);
	assert(!w1_rc1.meta);
	assert(!w2_heap.meta);

	// wc decremented
	assert(b0_wc2.wc == 1);
	assert(b1_rc1.wc == 0);
	// heap_box freed by sptr_deletew
}

void test_sptr_deletew(void) {
	test_sptr_deletew_errors();
	test_sptr_deletew_ok();
}

// deleteo
static void test_sptr_deleteo_errors(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_fbox b0_borrow_no_wc =
	    (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b1_owner_rc0 =
	    (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_fbox b2_owner_rc2 =
	    (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 0};

	t_owned_ptr o0_borrow_no_wc =
	    (t_owned_ptr){b0_borrow_no_wc.data, &b0_borrow_no_wc, false};
	t_owned_ptr o1_owner_rc0 =
	    (t_owned_ptr){b1_owner_rc0.data, &b1_owner_rc0, true};
	t_owned_ptr o2_owner_rc2 =
	    (t_owned_ptr){b2_owner_rc2.data, &b2_owner_rc2, true};
	t_owned_ptr o3_meta_null = (t_owned_ptr){data, NULL, true};

	bool res[] = {
	    sptr_deleteo(NULL),          sptr_deleteo(&o0_borrow_no_wc),
	    sptr_deleteo(&o1_owner_rc0), sptr_deleteo(&o2_owner_rc2),
	    sptr_deleteo(&o3_meta_null),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
}

static void test_sptr_deleteo_ok(void) {
	void *data = (void *)0xDEAD;
	void *dt   = dummy_dt;

	t_fbox  b0_own     = (t_fbox){.data = data, .dtor = dt, .rc = 1, .wc = 1};
	t_fbox  b1_bor     = (t_fbox){.data = data, .dtor = dt, .rc = 1, .wc = 1};
	t_fbox  b2_wc_many = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 2};
	t_fbox *b3_hp_own  = malloc(sizeof(*b3_hp_own));
	t_fbox *b4_hp_bor  = malloc(sizeof(*b4_hp_bor));
	assert(b3_hp_own != NULL && b4_hp_bor != NULL);
	*b3_hp_own = (t_fbox){.data = data, .dtor = dt, .rc = 1, .wc = 0};
	*b4_hp_bor = (t_fbox){.data = NULL, .dtor = NULL, .rc = 0, .wc = 1};

	t_owned_ptr o0_owner    = (t_owned_ptr){data, &b0_own, true};
	t_owned_ptr o1_borrow   = (t_owned_ptr){data, &b1_bor, false};
	t_owned_ptr o2_wc_many  = (t_owned_ptr){data, &b2_wc_many, true};
	t_owned_ptr o3_heap_own = (t_owned_ptr){data, b3_hp_own, true};
	t_owned_ptr o4_heap_bor = (t_owned_ptr){data, b4_hp_bor, false};

	g_dtor_calls = 0;

	bool res[] = {sptr_deleteo(&o0_owner), sptr_deleteo(&o1_borrow),
	              sptr_deleteo(&o2_wc_many), sptr_deleteo(&o3_heap_own),
	              sptr_deleteo(&o4_heap_bor)};

	assert(res[0] == true);
	assert(res[1] == true);
	assert(res[2] == true);
	assert(res[3] == true);
	assert(res[4] == true);

	assert(!o0_owner.data && !o0_owner.meta && !o0_owner.owner);
	assert(!o1_borrow.data && !o1_borrow.meta && !o1_borrow.owner);
	assert(!o2_wc_many.data && !o2_wc_many.meta && !o2_wc_many.owner);
	assert(!o3_heap_own.data && !o3_heap_own.meta && !o3_heap_own.owner);
	assert(!o4_heap_bor.data && !o4_heap_bor.meta && !o4_heap_bor.owner);

	assert(b0_own.rc == 0 && b0_own.wc == 1);
	assert(!b0_own.data && !b0_own.dtor);
	assert(b1_bor.rc == 1 && b1_bor.wc == 0);
	assert(b2_wc_many.rc == 0 && b2_wc_many.wc == 2);
	assert(!b2_wc_many.data && !b2_wc_many.dtor);

	assert(g_dtor_calls == 2);
}

void test_sptr_deleteo(void) {
	test_sptr_deleteo_errors();
	test_sptr_deleteo_ok();
}

// deleteu
static void test_sptr_deleteu_errors(void) {
	t_uniq_ptr u0_data_null = (t_uniq_ptr){.data = NULL, .dtor = dummy_dt};

	bool res[] = {
	    sptr_deleteu(NULL),
	    sptr_deleteu(&u0_data_null),
	};

	assert(!res[0]);
	assert(!res[1]);
}

static void test_sptr_deleteu_ok(void) {
	void *data = (void *)0xDEAD;

	t_uniq_ptr u0_with_dtor = (t_uniq_ptr){.data = data, .dtor = dummy_dt};
	t_uniq_ptr u1_no_dtor   = (t_uniq_ptr){.data = data, .dtor = NULL};

	g_dtor_calls = 0;

	bool res[] = {
	    sptr_deleteu(&u0_with_dtor),
	    sptr_deleteu(&u1_no_dtor),
	};

	assert(res[0] == true);
	assert(res[1] == true);

	assert(!u0_with_dtor.data && !u0_with_dtor.dtor);
	assert(!u1_no_dtor.data && !u1_no_dtor.dtor);

	assert(g_dtor_calls == 1);
}

void test_sptr_deleteu(void) {
	test_sptr_deleteu_errors();
	test_sptr_deleteu_ok();
}
