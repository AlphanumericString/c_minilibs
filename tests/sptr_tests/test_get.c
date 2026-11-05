#include <assert.h>

#include "sptr_tests.h"

static void local_dtor(void *_) {
	return;
}

// const-data
NO_COV void test_sptr_getsc(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	const t_cm_fbox b0_ok =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	const t_cm_fbox b1_no_rc =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};

	const t_cm_strong_ptr s0_ok =
	    (const t_cm_strong_ptr){b0_ok.data, (t_cm_fbox *)&b0_ok};
	const t_cm_strong_ptr s1_no_rc =
	    (const t_cm_strong_ptr){b1_no_rc.data, (t_cm_fbox *)&b1_no_rc};
	const t_cm_strong_ptr s2_meta_null = (const t_cm_strong_ptr){data, NULL};

	const void *res[] = {
	    sptr_getsc(&s0_ok),
	    sptr_getsc(&s1_no_rc),
	    sptr_getsc(&s2_meta_null),
	    sptr_getsc(NULL),
	};

	assert(res[0] == data);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
}

NO_COV void test_sptr_getwc(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	const t_cm_fbox b0_ok =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	const t_cm_fbox b1_no_rc =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};

	const t_cm_weak_ptr w0_ok = (const t_cm_weak_ptr){(t_cm_fbox *)&b0_ok};
	const t_cm_weak_ptr w1_no_rc =
	    (const t_cm_weak_ptr){(t_cm_fbox *)&b1_no_rc};
	const t_cm_weak_ptr w2_meta_null = (const t_cm_weak_ptr){NULL};

	const void *res[] = {
	    sptr_getwc(&w0_ok),
	    sptr_getwc(&w1_no_rc),
	    sptr_getwc(&w2_meta_null),
	    sptr_getwc(NULL),
	};

	assert(res[0] == data);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
}

NO_COV void test_sptr_getoc(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	const t_cm_fbox b0_rc1 =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	const t_cm_fbox b1_rc0 =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	const t_cm_fbox b2_rc2 =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 0};

	const t_cm_owned_ptr o0_rc1 =
	    (const t_cm_owned_ptr){b0_rc1.data, (t_cm_fbox *)&b0_rc1, true};
	const t_cm_owned_ptr o1_rc0 =
	    (const t_cm_owned_ptr){b1_rc0.data, (t_cm_fbox *)&b1_rc0, false};
	const t_cm_owned_ptr o2_rc2 =
	    (const t_cm_owned_ptr){b2_rc2.data, (t_cm_fbox *)&b2_rc2, true};
	const t_cm_owned_ptr o3_meta_null =
	    (const t_cm_owned_ptr){data, NULL, true};

	const void *res[] = {
	    sptr_getoc(&o0_rc1),       sptr_getoc(&o1_rc0), sptr_getoc(&o2_rc2),
	    sptr_getoc(&o3_meta_null), sptr_getoc(NULL),
	};

	assert(res[0] == data);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
}

NO_COV void test_sptr_getuc(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	const t_cm_uniq_ptr u0_ok =
	    (const t_cm_uniq_ptr){.data = data, .dtor = dtor};
	const t_cm_uniq_ptr u1_data_null =
	    (const t_cm_uniq_ptr){.data = NULL, .dtor = dtor};

	const void *res[] = {
	    sptr_getuc(&u0_ok),
	    sptr_getuc(&u1_data_null),
	    sptr_getuc(NULL),
	};

	assert(res[0] == data);
	assert(!res[1]);
	assert(!res[2]);
}

// non-const data
NO_COV void test_sptr_gets(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_fbox b0_ok = (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_cm_fbox b1_no_rc =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};

	t_cm_strong_ptr s0_ok        = (t_cm_strong_ptr){b0_ok.data, &b0_ok};
	t_cm_strong_ptr s1_no_rc     = (t_cm_strong_ptr){b1_no_rc.data, &b1_no_rc};
	t_cm_strong_ptr s2_meta_null = (t_cm_strong_ptr){data, NULL};

	void *res[] = {
	    sptr_gets(&s0_ok),
	    sptr_gets(&s1_no_rc),
	    sptr_gets(&s2_meta_null),
	    sptr_gets(NULL),
	};

	assert(res[0] == data);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
}

NO_COV void test_sptr_getw(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_fbox b0_ok = (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_cm_fbox b1_no_rc =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};

	t_cm_weak_ptr w0_ok        = (t_cm_weak_ptr){&b0_ok};
	t_cm_weak_ptr w1_no_rc     = (t_cm_weak_ptr){&b1_no_rc};
	t_cm_weak_ptr w2_meta_null = (t_cm_weak_ptr){NULL};

	void *res[] = {
	    sptr_getw(&w0_ok),
	    sptr_getw(&w1_no_rc),
	    sptr_getw(&w2_meta_null),
	    sptr_getw(NULL),
	};

	assert(res[0] == data);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
}

NO_COV void test_sptr_geto(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_fbox b0_rc1 =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_cm_fbox b1_rc0 =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_cm_fbox b2_rc2 =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 0};

	t_cm_owned_ptr o0_rc1       = (t_cm_owned_ptr){b0_rc1.data, &b0_rc1, true};
	t_cm_owned_ptr o1_rc0       = (t_cm_owned_ptr){b1_rc0.data, &b1_rc0, false};
	t_cm_owned_ptr o2_rc2       = (t_cm_owned_ptr){b2_rc2.data, &b2_rc2, true};
	t_cm_owned_ptr o3_meta_null = (t_cm_owned_ptr){data, NULL, true};

	void *res[] = {
	    sptr_geto(&o0_rc1),       sptr_geto(&o1_rc0), sptr_geto(&o2_rc2),
	    sptr_geto(&o3_meta_null), sptr_geto(NULL),
	};

	assert(res[0] == data);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
}

NO_COV void test_sptr_getu(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_uniq_ptr u0_ok        = (t_cm_uniq_ptr){.data = data, .dtor = dtor};
	t_cm_uniq_ptr u1_data_null = (t_cm_uniq_ptr){.data = NULL, .dtor = dtor};

	void *res[] = {
	    sptr_getu(&u0_ok),
	    sptr_getu(&u1_data_null),
	    sptr_getu(NULL),
	};

	assert(res[0] == data);
	assert(!res[1]);
	assert(!res[2]);
}

// ref-count
NO_COV void test_sptr_get_rcs(void) {
	void *data = (void *)0xDEAD;

	t_cm_fbox b0 = (t_cm_fbox){.data = data, .rc = 1, .wc = 0};
	t_cm_fbox b1 = (t_cm_fbox){.data = data, .rc = 42, .wc = 0};
	t_cm_fbox b2 = (t_cm_fbox){.data = data, .rc = 0, .wc = 0};

	t_cm_strong_ptr s0_rc1       = (t_cm_strong_ptr){b0.data, &b0};
	t_cm_strong_ptr s1_rc42      = (t_cm_strong_ptr){b1.data, &b1};
	t_cm_strong_ptr s2_rc0       = (t_cm_strong_ptr){b2.data, &b2};
	t_cm_strong_ptr s3_meta_null = (t_cm_strong_ptr){data, NULL};

	size_t res[] = {
	    sptr_get_strongcount_s(&s0_rc1), sptr_get_strongcount_s(&s1_rc42),
	    sptr_get_strongcount_s(&s2_rc0), sptr_get_strongcount_s(&s3_meta_null),
	    sptr_get_strongcount_s(NULL),
	};

	assert(res[0] == 1);
	assert(res[1] == 42);
	assert(res[2] == 0);
	assert(res[3] == 0);
	assert(res[4] == 0);

	// Also check macro
	assert(sptr_get_rc(&s0_rc1) == 1);
}

NO_COV void test_sptr_get_rcw(void) {
	void *data = (void *)0xDEAD;

	t_cm_fbox b0 = (t_cm_fbox){.data = data, .rc = 1, .wc = 1};
	t_cm_fbox b1 = (t_cm_fbox){.data = data, .rc = 99, .wc = 1};
	t_cm_fbox b2 = (t_cm_fbox){.data = data, .rc = 0, .wc = 1};

	t_cm_weak_ptr w0_rc1       = (t_cm_weak_ptr){&b0};
	t_cm_weak_ptr w1_rc99      = (t_cm_weak_ptr){&b1};
	t_cm_weak_ptr w2_rc0       = (t_cm_weak_ptr){&b2};
	t_cm_weak_ptr w3_meta_null = (t_cm_weak_ptr){NULL};

	size_t res[] = {
	    sptr_get_strongcount_w(&w0_rc1), sptr_get_strongcount_w(&w1_rc99),
	    sptr_get_strongcount_w(&w2_rc0), sptr_get_strongcount_w(&w3_meta_null),
	    sptr_get_strongcount_w(NULL),
	};

	assert(res[0] == 1);
	assert(res[1] == 99);
	assert(res[2] == 0);
	assert(res[3] == 0);
	assert(res[4] == 0);

	assert(sptr_get_rc(&w0_rc1) == 1);
}

NO_COV void test_sptr_get_rco(void) {
	void *data = (void *)0xDEAD;

	t_cm_fbox b0 = (t_cm_fbox){.data = data, .rc = 1, .wc = 0};
	t_cm_fbox b1 = (t_cm_fbox){.data = data, .rc = 17, .wc = 0};
	t_cm_fbox b2 = (t_cm_fbox){.data = data, .rc = 0, .wc = 0};

	t_cm_owned_ptr o0_rc1       = (t_cm_owned_ptr){b0.data, &b0, true};
	t_cm_owned_ptr o1_rc17      = (t_cm_owned_ptr){b1.data, &b1, false};
	t_cm_owned_ptr o2_rc0       = (t_cm_owned_ptr){b2.data, &b2, false};
	t_cm_owned_ptr o3_meta_null = (t_cm_owned_ptr){data, NULL, true};

	size_t res[] = {
	    sptr_get_strongcount_o(&o0_rc1), sptr_get_strongcount_o(&o1_rc17),
	    sptr_get_strongcount_o(&o2_rc0), sptr_get_strongcount_o(&o3_meta_null),
	    sptr_get_strongcount_o(NULL),
	};

	assert(res[0] == 1);
	assert(res[1] == 17);
	assert(res[2] == 0);
	assert(res[3] == 0);
	assert(res[4] == 0);

	assert(sptr_get_rc(&o0_rc1) == 1);
}

NO_COV void test_sptr_get_rcu(void) {
	void *data = (void *)0xDEAD;

	t_cm_uniq_ptr u0_ok        = (t_cm_uniq_ptr){.data = data, .dtor = NULL};
	t_cm_uniq_ptr u1_data_null = (t_cm_uniq_ptr){.data = NULL, .dtor = NULL};

	size_t res[] = {
	    sptr_get_strongcount_u(&u0_ok),
	    sptr_get_strongcount_u(&u1_data_null),
	    sptr_get_strongcount_u(NULL),
	};

	assert(res[0] == 1);
	assert(res[1] == 0);
	assert(res[2] == 0);

	assert(sptr_get_rc(&u0_ok) == 1);
}

// weak-count
NO_COV void test_sptr_get_wcs(void) {
	void *data = (void *)0xDEAD;

	t_cm_fbox b0_wc0   = (t_cm_fbox){.data = data, .rc = 1, .wc = 0};
	t_cm_fbox b1_wc5   = (t_cm_fbox){.data = data, .rc = 1, .wc = 5};
	t_cm_fbox b2_no_rc = (t_cm_fbox){.data = data, .rc = 0, .wc = 5};

	t_cm_strong_ptr s0_wc0       = (t_cm_strong_ptr){b0_wc0.data, &b0_wc0};
	t_cm_strong_ptr s1_wc5       = (t_cm_strong_ptr){b1_wc5.data, &b1_wc5};
	t_cm_strong_ptr s2_no_rc     = (t_cm_strong_ptr){b2_no_rc.data, &b2_no_rc};
	t_cm_strong_ptr s3_meta_null = (t_cm_strong_ptr){data, NULL};

	size_t res[] = {
	    sptr_get_weakcount_s(&s0_wc0),   sptr_get_weakcount_s(&s1_wc5),
	    sptr_get_weakcount_s(&s2_no_rc), sptr_get_weakcount_s(&s3_meta_null),
	    sptr_get_weakcount_s(NULL),
	};

	assert(res[0] == 0);
	assert(res[1] == 5);
	assert(res[2] == 0); // rc == 0 causes sptr_get_weakcount_s to return 0
	assert(res[3] == 0);
	assert(res[4] == 0);

	assert(sptr_get_wc(&s1_wc5) == 5);
}

NO_COV void test_sptr_get_wcw(void) {
	void *data = (void *)0xDEAD;

	t_cm_fbox b0_wc1   = (t_cm_fbox){.data = data, .rc = 1, .wc = 1};
	t_cm_fbox b1_wc10  = (t_cm_fbox){.data = data, .rc = 1, .wc = 10};
	t_cm_fbox b2_no_wc = (t_cm_fbox){.data = data, .rc = 1, .wc = 0};

	t_cm_weak_ptr w0_wc1       = (t_cm_weak_ptr){&b0_wc1};
	t_cm_weak_ptr w1_wc10      = (t_cm_weak_ptr){&b1_wc10};
	t_cm_weak_ptr w2_no_wc     = (t_cm_weak_ptr){&b2_no_wc};
	t_cm_weak_ptr w3_meta_null = (t_cm_weak_ptr){NULL};

	size_t res[] = {
	    sptr_get_weakcount_w(&w0_wc1),   sptr_get_weakcount_w(&w1_wc10),
	    sptr_get_weakcount_w(&w2_no_wc), sptr_get_weakcount_w(&w3_meta_null),
	    sptr_get_weakcount_w(NULL),
	};

	assert(res[0] == 1);
	assert(res[1] == 10);
	assert(res[2] == 0);
	assert(res[3] == 0);
	assert(res[4] == 0);

	assert(sptr_get_wc(&w1_wc10) == 10);
}

NO_COV void test_sptr_get_wco(void) {
	void *data = (void *)0xDEAD;

	t_cm_fbox b0_wc0 = (t_cm_fbox){.data = data, .rc = 1, .wc = 0};
	t_cm_fbox b1_wc8 = (t_cm_fbox){.data = data, .rc = 1, .wc = 8};

	t_cm_owned_ptr o0_wc0       = (t_cm_owned_ptr){b0_wc0.data, &b0_wc0, true};
	t_cm_owned_ptr o1_wc8       = (t_cm_owned_ptr){b1_wc8.data, &b1_wc8, false};
	t_cm_owned_ptr o2_meta_null = (t_cm_owned_ptr){data, NULL, true};

	size_t res[] = {
	    sptr_get_weakcount_o(&o0_wc0),
	    sptr_get_weakcount_o(&o1_wc8),
	    sptr_get_weakcount_o(&o2_meta_null),
	    sptr_get_weakcount_o(NULL),
	};

	assert(res[0] == 0);
	assert(res[1] == 8);
	assert(res[2] == 0);
	assert(res[3] == 0);

	assert(sptr_get_wc(&o1_wc8) == 8);
}

NO_COV void test_sptr_get_wcu(void) {
	void *data = (void *)0xDEAD;

	t_cm_uniq_ptr u0_ok        = (t_cm_uniq_ptr){.data = data, .dtor = NULL};
	t_cm_uniq_ptr u1_data_null = (t_cm_uniq_ptr){.data = NULL, .dtor = NULL};

	size_t res[] = {
	    sptr_get_weakcount_u(&u0_ok),
	    sptr_get_weakcount_u(&u1_data_null),
	    sptr_get_weakcount_u(NULL),
	};

	assert(res[0] == 0);
	assert(res[1] == 0);
	assert(res[2] == 0);

	assert(sptr_get_wc(&u0_ok) == 0);
}

// dtors
NO_COV void test_sptr_get_dtors(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_fbox b0_dtor =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_cm_fbox b1_null =
	    (t_cm_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 0};
	t_cm_fbox b2_no_rc =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};

	t_cm_strong_ptr s0_dtor      = (t_cm_strong_ptr){b0_dtor.data, &b0_dtor};
	t_cm_strong_ptr s1_null      = (t_cm_strong_ptr){b1_null.data, &b1_null};
	t_cm_strong_ptr s2_no_rc     = (t_cm_strong_ptr){b2_no_rc.data, &b2_no_rc};
	t_cm_strong_ptr s3_meta_null = (t_cm_strong_ptr){data, NULL};

	t_cm_dtor res[] = {
	    sptr_get_dtors(&s0_dtor),  sptr_get_dtors(&s1_null),
	    sptr_get_dtors(&s2_no_rc), sptr_get_dtors(&s3_meta_null),
	    sptr_get_dtors(NULL),
	};

	assert(res[0] == dtor);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
}

NO_COV void test_sptr_get_dtorw(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_fbox b0_dtor =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_cm_fbox b1_null =
	    (t_cm_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};
	t_cm_fbox b2_no_wc =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_cm_fbox b3_no_rc =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};

	t_cm_weak_ptr w0_dtor      = (t_cm_weak_ptr){&b0_dtor};
	t_cm_weak_ptr w1_null      = (t_cm_weak_ptr){&b1_null};
	t_cm_weak_ptr w2_no_wc     = (t_cm_weak_ptr){&b2_no_wc};
	t_cm_weak_ptr w3_no_rc     = (t_cm_weak_ptr){&b3_no_rc};
	t_cm_weak_ptr w4_meta_null = (t_cm_weak_ptr){NULL};

	t_cm_dtor res[] = {
	    sptr_get_dtorw(&w0_dtor),      sptr_get_dtorw(&w1_null),
	    sptr_get_dtorw(&w2_no_wc),     sptr_get_dtorw(&w3_no_rc),
	    sptr_get_dtorw(&w4_meta_null), sptr_get_dtorw(NULL),
	};

	assert(res[0] == dtor);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
	assert(!res[5]);
}

NO_COV void test_sptr_get_dtoro(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_fbox b0_dtor =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_cm_fbox b1_null =
	    (t_cm_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 0};
	t_cm_fbox b2_no_rc =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};

	t_cm_owned_ptr o0_dtor  = (t_cm_owned_ptr){b0_dtor.data, &b0_dtor, true};
	t_cm_owned_ptr o1_null  = (t_cm_owned_ptr){b1_null.data, &b1_null, true};
	t_cm_owned_ptr o2_no_rc = (t_cm_owned_ptr){b2_no_rc.data, &b2_no_rc, false};
	t_cm_owned_ptr o3_meta_null = (t_cm_owned_ptr){data, NULL, true};

	t_cm_dtor res[] = {
	    sptr_get_dtoro(&o0_dtor),  sptr_get_dtoro(&o1_null),
	    sptr_get_dtoro(&o2_no_rc), sptr_get_dtoro(&o3_meta_null),
	    sptr_get_dtoro(NULL),
	};

	assert(res[0] == dtor);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
}

NO_COV void test_sptr_get_dtoru(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_uniq_ptr u0_dtor = (t_cm_uniq_ptr){.data = data, .dtor = dtor};
	t_cm_uniq_ptr u1_null = (t_cm_uniq_ptr){.data = data, .dtor = NULL};

	t_cm_dtor res[] = {
	    sptr_get_dtoru(&u0_dtor),
	    sptr_get_dtoru(&u1_null),
	    sptr_get_dtoru(NULL),
	};

	assert(res[0] == dtor);
	assert(!res[1]);
	assert(!res[2]);
}
