#include <assert.h>

#define C_MINI__INTERNAL
#include "sptr_tests.h"

// copys
static void test_sptr_copys_errors(void) {
	void *data = (void *)0xDEAD;

	t_fbox b0_no_rc = (t_fbox){.data = data, .rc = 0, .wc = 0};

	t_strong_ptr s0_no_rc     = (t_strong_ptr){b0_no_rc.data, &b0_no_rc};
	t_strong_ptr s1_meta_null = (t_strong_ptr){data, NULL};

	t_strong_ptr *res[] = {
	    sptr_copys(NULL),
	    sptr_copys(&s0_no_rc),
	    sptr_copys(&s1_meta_null),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
}

static void test_sptr_copys_ok(void) {
	void *data = (void *)0xDEAD;

	t_fbox b0 = (t_fbox){.data = data, .rc = 1, .wc = 0};
	t_fbox b1 = (t_fbox){.data = data, .rc = 5, .wc = 2};

	t_strong_ptr s0 = (t_strong_ptr){b0.data, &b0};
	t_strong_ptr s1 = (t_strong_ptr){b1.data, &b1};

	t_strong_ptr *res[] = {
	    sptr_copys(&s0),
	    sptr_copys(&s1),
	};

	assert(res[0] == &s0);
	assert(res[1] == &s1);

	assert(b0.rc == 2);
	assert(b1.rc == 6);
}

void test_sptr_copys(void) {
	test_sptr_copys_errors();
	test_sptr_copys_ok();
}

// copyw
static void test_sptr_copyw_errors(void) {
	void *data = (void *)0xDEAD;

	t_fbox b0_no_wc = (t_fbox){.data = data, .rc = 1, .wc = 0};

	t_weak_ptr w0_no_wc     = (t_weak_ptr){&b0_no_wc};
	t_weak_ptr w1_meta_null = (t_weak_ptr){NULL};

	t_weak_ptr *res[] = {
	    sptr_copyw(NULL),
	    sptr_copyw(&w0_no_wc),
	    sptr_copyw(&w1_meta_null),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
}

static void test_sptr_copyw_ok(void) {
	void *data = (void *)0xDEAD;

	t_fbox b0 = (t_fbox){.data = data, .rc = 1, .wc = 1};
	t_fbox b1 = (t_fbox){.data = data, .rc = 1, .wc = 9};

	t_weak_ptr w0 = (t_weak_ptr){&b0};
	t_weak_ptr w1 = (t_weak_ptr){&b1};

	t_weak_ptr *res[] = {
	    sptr_copyw(&w0),
	    sptr_copyw(&w1),
	};

	assert(res[0] == &w0);
	assert(res[1] == &w1);

	assert(b0.wc == 2);
	assert(b1.wc == 10);
}

void test_sptr_copyw(void) {
	test_sptr_copyw_errors();
	test_sptr_copyw_ok();
}

// copyo
static void test_sptr_copyo_errors(void) {
	void *data = (void *)0xDEAD;

	t_fbox b0_no_rc = (t_fbox){.data = data, .rc = 0, .wc = 0};

	t_owned_ptr o0_no_rc     = (t_owned_ptr){b0_no_rc.data, &b0_no_rc, true};
	t_owned_ptr o1_meta_null = (t_owned_ptr){data, NULL, true};

	t_owned_ptr res[] = {
	    sptr_copyo(NULL),
	    sptr_copyo(&o0_no_rc),
	    sptr_copyo(&o1_meta_null),
	};

	assert(!res[0].data && !res[0].meta && !res[0].owner);
	assert(!res[1].data && !res[1].meta && !res[1].owner);
	assert(!res[2].data && !res[2].meta && !res[2].owner);
}

static void test_sptr_copyo_ok(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_fbox b0 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b1 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 3};

	t_owned_ptr o0_owner    = (t_owned_ptr){b0.data, &b0, true};
	t_owned_ptr o1_borrower = (t_owned_ptr){b1.data, &b1, false};

	t_owned_ptr res[] = {
	    sptr_copyo(&o0_owner),
	    sptr_copyo(&o1_borrower),
	};

	// Copies are non-owning borrowers
	assert(res[0].data == data && res[0].meta == &b0 && res[0].owner == false);
	assert(res[1].data == data && res[1].meta == &b1 && res[1].owner == false);

	// Original ownership status unchanged
	assert(o0_owner.owner == true);
	assert(!o1_borrower.owner);

	// wc incremented on boxes
	assert(b0.wc == 1);
	assert(b1.wc == 4);
}

void test_sptr_copyo(void) {
	test_sptr_copyo_errors();
	test_sptr_copyo_ok();
}

// copyu
static void test_sptr_copyu_errors(void) {
	t_uniq_ptr res[] = {
	    sptr_copyu(NULL),
	};

	assert(!res[0].data && !res[0].dtor);
}

static void test_sptr_copyu_ok(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_uniq_ptr u0 = (t_uniq_ptr){.data = data, .dtor = dtor};
	t_uniq_ptr u1 = (t_uniq_ptr){.data = data, .dtor = NULL};

	t_uniq_ptr res[] = {
	    sptr_copyu(&u0),
	    sptr_copyu(&u1),
	};

	assert(res[0].data == data && res[0].dtor == dtor);
	assert(res[1].data == data && res[1].dtor == NULL);

	// Original unique pointers zeroed out
	assert(!u0.data && !u0.dtor);
	assert(!u1.data && !u1.dtor);
}

void test_sptr_copyu(void) {
	test_sptr_copyu_errors();
	test_sptr_copyu_ok();
}
