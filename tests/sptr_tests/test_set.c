#include <assert.h>

#define C_MINI__INTERNAL
#include "sptr_tests.h"

// set_dtors
static void test_sptr_set_dtors_errors(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_fbox b0_no_rc = (t_fbox){.data = data, .dtor = NULL, .rc = 0, .wc = 0};

	t_strong_ptr s0_no_rc     = (t_strong_ptr){b0_no_rc.data, &b0_no_rc};
	t_strong_ptr s1_meta_null = (t_strong_ptr){data, NULL};

	bool res[] = {
	    sptr_set_dtors(NULL, dtor),
	    sptr_set_dtors(&s0_no_rc, dtor),
	    sptr_set_dtors(&s1_meta_null, dtor),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
}

static void test_sptr_set_dtors_ok(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_fbox b0 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 0};
	t_fbox b1 = (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 0};

	t_strong_ptr s0 = (t_strong_ptr){b0.data, &b0};
	t_strong_ptr s1 = (t_strong_ptr){b1.data, &b1};

	bool res[] = {
	    sptr_set_dtors(&s0, dtor),
	    sptr_set_dtors(&s1, NULL),
	};

	assert(res[0] == true);
	assert(res[1] == true);

	assert(b0.dtor == dtor);
	assert(b1.dtor == NULL);
}

void test_sptr_set_dtors(void) {
	test_sptr_set_dtors_errors();
	test_sptr_set_dtors_ok();
}

// set_dtorw (always returns false)
void test_sptr_set_dtorw(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_fbox b0_ok    = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};
	t_fbox b1_no_wc = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 0};
	t_fbox b2_no_rc = (t_fbox){.data = data, .dtor = NULL, .rc = 0, .wc = 1};

	t_weak_ptr w0_ok        = (t_weak_ptr){&b0_ok};
	t_weak_ptr w1_no_wc     = (t_weak_ptr){&b1_no_wc};
	t_weak_ptr w2_no_rc     = (t_weak_ptr){&b2_no_rc};
	t_weak_ptr w3_meta_null = (t_weak_ptr){NULL};

	bool res[] = {
	    sptr_set_dtorw(&w0_ok, dtor),    sptr_set_dtorw(&w1_no_wc, dtor),
	    sptr_set_dtorw(&w2_no_rc, dtor), sptr_set_dtorw(&w3_meta_null, dtor),
	    sptr_set_dtorw(NULL, dtor),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);

	// dtor never modified
	assert(!b0_ok.dtor);
}

// set_dtoro
static void test_sptr_set_dtoro_errors(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_fbox b0_borrow = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};
	t_fbox b1_rc0    = (t_fbox){.data = data, .dtor = NULL, .rc = 0, .wc = 0};
	t_fbox b2_rc2    = (t_fbox){.data = data, .dtor = NULL, .rc = 2, .wc = 0};

	t_owned_ptr o0_borrow    = (t_owned_ptr){b0_borrow.data, &b0_borrow, false};
	t_owned_ptr o1_rc0       = (t_owned_ptr){b1_rc0.data, &b1_rc0, true};
	t_owned_ptr o2_rc2       = (t_owned_ptr){b2_rc2.data, &b2_rc2, true};
	t_owned_ptr o3_meta_null = (t_owned_ptr){data, NULL, true};

	bool res[] = {
	    sptr_set_dtoro(NULL, dtor),          sptr_set_dtoro(&o0_borrow, dtor),
	    sptr_set_dtoro(&o1_rc0, dtor),       sptr_set_dtoro(&o2_rc2, dtor),
	    sptr_set_dtoro(&o3_meta_null, dtor),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
}

static void test_sptr_set_dtoro_ok(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_fbox b0 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 0};
	t_fbox b1 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};

	t_owned_ptr o0 = (t_owned_ptr){b0.data, &b0, true};
	t_owned_ptr o1 = (t_owned_ptr){b1.data, &b1, true};

	bool res[] = {
	    sptr_set_dtoro(&o0, dtor),
	    sptr_set_dtoro(&o1, NULL),
	};

	assert(res[0] == true);
	assert(res[1] == true);

	assert(b0.dtor == dtor);
	assert(b1.dtor == NULL);
}

void test_sptr_set_dtoro(void) {
	test_sptr_set_dtoro_errors();
	test_sptr_set_dtoro_ok();
}

// set_dtoru
static void test_sptr_set_dtoru_errors(void) {
	void *dtor = (void *)0xCAFE;

	t_uniq_ptr u0_data_null = (t_uniq_ptr){.data = NULL, .dtor = NULL};

	bool res[] = {
	    sptr_set_dtoru(NULL, dtor),
	    sptr_set_dtoru(&u0_data_null, dtor),
	};

	assert(!res[0]);
	assert(!res[1]);
}

static void test_sptr_set_dtoru_ok(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_uniq_ptr u0 = (t_uniq_ptr){.data = data, .dtor = NULL};
	t_uniq_ptr u1 = (t_uniq_ptr){.data = data, .dtor = dtor};

	bool res[] = {
	    sptr_set_dtoru(&u0, dtor),
	    sptr_set_dtoru(&u1, NULL),
	};

	assert(res[0] == true);
	assert(res[1] == true);

	assert(u0.dtor == dtor);
	assert(u1.dtor == NULL);
}

void test_sptr_set_dtoru(void) {
	test_sptr_set_dtoru_errors();
	test_sptr_set_dtoru_ok();
}
