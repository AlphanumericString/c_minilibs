#include <assert.h>
#include <stdlib.h>

#include "sptr_tests.h"

// make_owned_from_raw
static void test_sptr_make_owned_from_raw_errors(void) {
	void *dtor = (void *)0xCAFE;

	t_owned_ptr res[] = {
	    sptr_make_owned_from_raw(NULL, NULL),
	    sptr_make_owned_from_raw(NULL, dtor),
	};

	assert(!res[0].data && !res[0].meta && !res[0].owner);
	assert(!res[1].data && !res[1].meta && !res[1].owner);
}

static void test_sptr_make_owned_from_raw_ok(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_owned_ptr res[] = {
	    sptr_make_owned_from_raw(data, NULL),
	    sptr_make_owned_from_raw(data, dtor),
	};

	assert(res[0].data == data && res[0].meta != NULL && res[0].owner == true);
	assert(res[0].data == res[0].meta->data);
	assert(sptr_get_rc(&res[0]) == 1 && sptr_get_wc(&res[0]) == 0);
	assert(sptr_get_dtor(&res[0]) == NULL);
	free(res[0].meta);

	assert(res[1].data == data && res[1].meta != NULL && res[1].owner == true);
	assert(res[1].data == res[1].meta->data);
	assert(sptr_get_rc(&res[1]) == 1 && sptr_get_wc(&res[1]) == 0);
	assert(sptr_get_dtor(&res[1]) == dtor);
	free(res[1].meta);
}

void test_sptr_make_owned_from_raw(void) {
	test_sptr_make_owned_from_raw_errors();
	test_sptr_make_owned_from_raw_ok();
}

// make_owned_from_strong
static void test_sptr_make_owned_from_strong_errors(void) {
	void *data     = (void *)0xDEAD;
	void *dtor     = (void *)0xCAFE;
	void *not_dtor = (void *)0xBEAD;

	t_fbox b0_no_rc   = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_fbox b1_rc_many = (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 1};
	t_fbox b2_dt_mismatch =
	    (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b3_meta_null =
	    (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};

	t_strong_ptr s0_no_rc   = (t_strong_ptr){b0_no_rc.data, &b0_no_rc};
	t_strong_ptr s1_rc_many = (t_strong_ptr){b1_rc_many.data, &b1_rc_many};
	t_strong_ptr s2_dt_mismatch =
	    (t_strong_ptr){b2_dt_mismatch.data, &b2_dt_mismatch};
	t_strong_ptr s3_meta_null = (t_strong_ptr){b3_meta_null.data, NULL};

	t_owned_ptr res[] = {
	    sptr_make_owned_from_strong(NULL, NULL),
	    sptr_make_owned_from_strong(NULL, dtor),
	    sptr_make_owned_from_strong(&s0_no_rc, NULL),
	    sptr_make_owned_from_strong(&s1_rc_many, NULL),
	    sptr_make_owned_from_strong(&s2_dt_mismatch, not_dtor),
	    sptr_make_owned_from_strong(&s3_meta_null, NULL),
	};

	assert(!res[0].data && !res[0].meta && !res[0].owner);
	assert(!res[1].data && !res[1].meta && !res[1].owner);
	assert(!res[2].data && !res[2].meta && !res[2].owner);
	assert(!res[3].data && !res[3].meta && !res[3].owner);
	assert(!res[4].data && !res[4].meta && !res[4].owner);
	assert(!res[5].data && !res[5].meta && !res[5].owner);
}

static void test_sptr_make_owned_from_strong_ok(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_fbox b0 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b1 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};
	t_fbox b2 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 2};
	t_fbox b3 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 10};

	t_strong_ptr s0_dt_match      = (t_strong_ptr){b0.data, &b0};
	t_strong_ptr s1_dt_override   = (t_strong_ptr){b1.data, &b1};
	t_strong_ptr s2_dt_left_as_is = (t_strong_ptr){b2.data, &b2};
	t_strong_ptr s3_dt_left_null  = (t_strong_ptr){b3.data, &b3};

	t_owned_ptr res[] = {
	    sptr_make_owned_from_strong(&s0_dt_match, dtor),
	    sptr_make_owned_from_strong(&s1_dt_override, dtor),
	    sptr_make_owned_from_strong(&s2_dt_left_as_is, NULL),
	    sptr_make_owned_from_strong(&s3_dt_left_null, NULL),
	};

	assert(res[0].data == data && res[0].meta == &b0 && res[0].owner == true);
	assert(res[1].data == data && res[1].meta == &b1 && res[1].owner == true);
	assert(res[2].data == data && res[2].meta == &b2 && res[2].owner == true);
	assert(res[3].data == data && res[3].meta == &b3 && res[3].owner == true);

	assert(!s0_dt_match.data && !s0_dt_match.meta);
	assert(!s1_dt_override.data && !s1_dt_override.meta);
	assert(!s2_dt_left_as_is.data && !s2_dt_left_as_is.meta);
	assert(!s3_dt_left_null.data && !s3_dt_left_null.meta);

	assert(sptr_get_rc(&res[0]) == 1 && sptr_get_wc(&res[0]) == 1);
	assert(sptr_get_rc(&res[1]) == 1 && sptr_get_wc(&res[1]) == 1);
	assert(sptr_get_rc(&res[2]) == 1 && sptr_get_wc(&res[2]) == 2);
	assert(sptr_get_rc(&res[3]) == 1 && sptr_get_wc(&res[3]) == 10);

	assert(sptr_get_dtor(&res[0]) == dtor);
	assert(sptr_get_dtor(&res[1]) == dtor);
	assert(sptr_get_dtor(&res[2]) == dtor);
	assert(sptr_get_dtor(&res[3]) == NULL);
}

void test_sptr_make_owned_from_strong(void) {
	test_sptr_make_owned_from_strong_errors();
	test_sptr_make_owned_from_strong_ok();
}

// make_owned_from_owned
static void test_sptr_make_owned_from_owned_errors(void) {
	void *data     = (void *)0xDEAD;
	void *dtor     = (void *)0xCAFE;
	void *not_dtor = (void *)0xBEAD;

	t_fbox b0_ok = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};

	t_owned_ptr o0_meta_null =
	    (t_owned_ptr){.data = data, .meta = NULL, .owner = true};
	t_owned_ptr o1_dt_mismatch =
	    (t_owned_ptr){.data = data, .meta = &b0_ok, .owner = true};

	t_owned_ptr res[] = {
	    sptr_make_owned_from_owned(NULL, NULL),
	    sptr_make_owned_from_owned(NULL, dtor),
	    sptr_make_owned_from_owned(&o0_meta_null, NULL),
	    sptr_make_owned_from_owned(&o1_dt_mismatch, not_dtor),
	};

	assert(!res[0].data && !res[0].meta && !res[0].owner);
	assert(!res[1].data && !res[1].meta && !res[1].owner);
	assert(!res[2].data && !res[2].meta && !res[2].owner);
	assert(!res[3].data && !res[3].meta && !res[3].owner);
}

static void test_sptr_make_owned_from_owned_ok(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_fbox b0 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b1 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};
	t_fbox b2 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 5};
	t_fbox b3 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 0};

	t_owned_ptr o0_dt_match =
	    (t_owned_ptr){.data = b0.data, .meta = &b0, .owner = true};
	t_owned_ptr o1_dt_override =
	    (t_owned_ptr){.data = b1.data, .meta = &b1, .owner = true};
	t_owned_ptr o2_dt_left_as_is =
	    (t_owned_ptr){.data = b2.data, .meta = &b2, .owner = false};
	t_owned_ptr o3_dt_left_null =
	    (t_owned_ptr){.data = b3.data, .meta = &b3, .owner = false};

	t_owned_ptr res[] = {
	    sptr_make_owned_from_owned(&o0_dt_match, dtor),
	    sptr_make_owned_from_owned(&o1_dt_override, dtor),
	    sptr_make_owned_from_owned(&o2_dt_left_as_is, NULL),
	    sptr_make_owned_from_owned(&o3_dt_left_null, NULL),
	};

	// Borrowed copy: owner is false
	assert(res[0].data == data && res[0].meta == &b0 && !res[0].owner);
	assert(res[1].data == data && res[1].meta == &b1 && !res[1].owner);
	assert(res[2].data == data && res[2].meta == &b2 && !res[2].owner);
	assert(res[3].data == data && res[3].meta == &b3 && !res[3].owner);

	// Original owned unchanged in ownership flag
	assert(o0_dt_match.owner == true);
	assert(o1_dt_override.owner == true);
	assert(!o2_dt_left_as_is.owner);
	assert(!o3_dt_left_null.owner);

	// wc incremented by 1
	assert(sptr_get_wc(&res[0]) == 1);
	assert(sptr_get_wc(&res[1]) == 2);
	assert(sptr_get_wc(&res[2]) == 6);
	assert(sptr_get_wc(&res[3]) == 1);

	// dtor management
	assert(sptr_get_dtor(&res[0]) == dtor);
	assert(sptr_get_dtor(&res[1]) == dtor);
	assert(sptr_get_dtor(&res[2]) == dtor);
	assert(sptr_get_dtor(&res[3]) == NULL);
}

void test_sptr_make_owned_from_owned(void) {
	test_sptr_make_owned_from_owned_errors();
	test_sptr_make_owned_from_owned_ok();
}

// make_owned_from_uniq
static void test_sptr_make_owned_from_uniq_errors(void) {
	void *data     = (void *)0xDEAD;
	void *dtor     = (void *)0xCAFE;
	void *not_dtor = (void *)0xBEAD;

	t_uniq_ptr u0 = (t_uniq_ptr){.data = data, .dtor = dtor};

	t_owned_ptr res[] = {
	    sptr_make_owned_from_uniq(NULL, NULL),
	    sptr_make_owned_from_uniq(NULL, dtor),
	    sptr_make_owned_from_uniq(&u0, not_dtor),
	};

	assert(!res[0].data && !res[0].meta && !res[0].owner);
	assert(!res[1].data && !res[1].meta && !res[1].owner);
	assert(!res[2].data && !res[2].meta && !res[2].owner);
}

static void test_sptr_make_owned_from_uniq_ok(void) {
	void *data = (void *)0xDEAD;
	void *dtor = (void *)0xCAFE;

	t_uniq_ptr u0 = (t_uniq_ptr){.data = data, .dtor = dtor};
	t_uniq_ptr u1 = (t_uniq_ptr){.data = data, .dtor = dtor};
	t_uniq_ptr u2 = (t_uniq_ptr){.data = data, .dtor = NULL};
	t_uniq_ptr u3 = (t_uniq_ptr){.data = data, .dtor = NULL};

	t_owned_ptr res[] = {
	    sptr_make_owned_from_uniq(&u0, NULL),
	    sptr_make_owned_from_uniq(&u1, dtor),
	    sptr_make_owned_from_uniq(&u2, dtor),
	    sptr_make_owned_from_uniq(&u3, NULL),
	};

	assert(res[0].data == data && res[0].meta != NULL && res[0].owner == true);
	assert(res[1].data == data && res[1].meta != NULL && res[1].owner == true);
	assert(res[2].data == data && res[2].meta != NULL && res[2].owner == true);
	assert(res[3].data == data && res[3].meta != NULL && res[3].owner == true);

	// uptr was nulled out
	assert(!u0.data && !u0.dtor);
	assert(!u1.data && !u1.dtor);
	assert(!u2.data && !u2.dtor);
	assert(!u3.data && !u3.dtor);

	assert(sptr_get_rc(&res[0]) == 1 && sptr_get_wc(&res[0]) == 0);
	assert(sptr_get_rc(&res[1]) == 1 && sptr_get_wc(&res[1]) == 0);
	assert(sptr_get_rc(&res[2]) == 1 && sptr_get_wc(&res[2]) == 0);
	assert(sptr_get_rc(&res[3]) == 1 && sptr_get_wc(&res[3]) == 0);

	assert(sptr_get_dtor(&res[0]) == dtor);
	assert(sptr_get_dtor(&res[1]) == dtor);
	assert(sptr_get_dtor(&res[2]) == dtor);
	assert(sptr_get_dtor(&res[3]) == NULL);

	free(res[0].meta);
	free(res[1].meta);
	free(res[2].meta);
	free(res[3].meta);
}

void test_sptr_make_owned_from_uniq(void) {
	test_sptr_make_owned_from_uniq_errors();
	test_sptr_make_owned_from_uniq_ok();
}
