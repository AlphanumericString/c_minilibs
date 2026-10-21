#include <assert.h>
#include <stdlib.h>

#include "sptr_tests.h"

static void local_dtor(void *_) {
	return;
}
static void other_dtor(void *_) {
	return;
}

NO_COV void test_sptr_make_shared_from_raw(void) {
	void        *data = (void *)0xDEAD;
	t_dtor       dtor = local_dtor;
	t_strong_ptr s1   = sptr_make_shared(data, NULL);
	t_strong_ptr s2   = sptr_make_shared(data, dtor);
	t_strong_ptr s3   = sptr_make_shared(NULL, NULL);
	t_strong_ptr s4;

	alloc_set_call_limit(0);
	s4 = sptr_make_shared_from_raw(data, dtor);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	assert(s1.data == data && s1.meta != NULL);
	assert(s1.data == s1.meta->data);
	assert(sptr_get_rc(&s1) == 1 && sptr_get_wc(&s1) == 0);
	assert(sptr_get_dtor(&s1) == NULL);
	free(s1.meta);

	assert(s2.data == data && s2.meta != NULL);
	assert(s2.data == s2.meta->data);
	assert(sptr_get_rc(&s2) == 1 && sptr_get_wc(&s2) == 0);
	assert(sptr_get_dtor(&s2) == dtor);
	free(s2.meta);

	assert(!s3.data && !s3.meta);

	assert(!s4.data && !s4.meta);
}

static void test_sptr_make_shared_from_weak_errors(void) {
	void  *data     = (void *)0xDEAD;
	t_dtor dtor     = local_dtor;
	t_dtor not_dtor = other_dtor;

	t_fbox good_box  = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox no_rc_box = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 9};
	t_fbox no_c_box  = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};

	// dt = dtor a=arg, s=source, neq=not equal, none=null;
	t_weak_ptr w1_dt_as_neq = (t_weak_ptr){&good_box};
	t_weak_ptr w2_meta_none = (t_weak_ptr){NULL};
	t_weak_ptr w4_no_c      = (t_weak_ptr){&no_c_box};
	t_weak_ptr w5_no_rc     = (t_weak_ptr){&no_rc_box};

	t_strong_ptr res[] = {
	    sptr_make_shared_from_weak(&w1_dt_as_neq, not_dtor),
	    sptr_make_shared_from_weak(&w2_meta_none, NULL),
	    sptr_make_shared_from_weak(&w4_no_c, NULL),
	    sptr_make_shared_from_weak(&w5_no_rc, NULL),
	    sptr_make_shared_from_weak(NULL, dtor),
	    sptr_make_shared_from_weak(NULL, NULL),
	};

	// note : leave assert unrolled, faster to see which failed
	assert(!res[0].data && !res[0].meta);
	assert(!res[1].data && !res[1].meta);
	assert(!res[2].data && !res[2].meta);
	assert(!res[3].data && !res[3].meta);
	assert(!res[4].data && !res[4].meta);
	assert(!res[5].data && !res[5].meta);
}
static void test_sptr_make_shared_from_weak_ok(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b1 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};
	t_fbox b2 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};
	t_fbox b3 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b4 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 99};
	t_fbox b5 = (t_fbox){.data = data, .dtor = dtor, .rc = 99, .wc = 1};

	t_weak_ptr w0_dt_match         = (t_weak_ptr){&b0};
	t_weak_ptr w1_override_on_null = (t_weak_ptr){&b1};
	t_weak_ptr w2_left_to_null     = (t_weak_ptr){&b2};
	t_weak_ptr w3_wc_none          = (t_weak_ptr){&b3};
	t_weak_ptr w4_wc_many          = (t_weak_ptr){&b4};
	t_weak_ptr w5_rc_many          = (t_weak_ptr){&b5};

	t_strong_ptr res[] = {
	    sptr_make_shared_from_weak(&w0_dt_match, dtor),
	    sptr_make_shared_from_weak(&w1_override_on_null, dtor),
	    sptr_make_shared_from_weak(&w2_left_to_null, NULL),
	    sptr_make_shared_from_weak(&w3_wc_none, NULL),
	    sptr_make_shared_from_weak(&w4_wc_many, NULL),
	    sptr_make_shared_from_weak(&w5_rc_many, NULL),
	};

	// base
	assert(res[0].data == data && res[0].meta != NULL);
	assert(res[1].data == data && res[1].meta != NULL);
	assert(res[2].data == data && res[2].meta != NULL);
	assert(res[3].data == data && res[3].meta != NULL);
	assert(res[4].data == data && res[4].meta != NULL);
	assert(res[5].data == data && res[5].meta != NULL);

	// count
	assert(sptr_get_rc(&res[0]) == 2 && sptr_get_wc(&res[0]) == 1);
	assert(sptr_get_rc(&res[1]) == 2 && sptr_get_wc(&res[1]) == 1);
	assert(sptr_get_rc(&res[2]) == 2 && sptr_get_wc(&res[2]) == 1);
	assert(sptr_get_rc(&res[3]) == 2 && sptr_get_wc(&res[3]) == 0);
	assert(sptr_get_rc(&res[4]) == 2 && sptr_get_wc(&res[4]) == 99);
	assert(sptr_get_rc(&res[5]) == 100 && sptr_get_wc(&res[5]) == 1);

	// dt management
	assert(sptr_get_dtor(&res[0]) == dtor);
	assert(sptr_get_dtor(&res[1]) == dtor);
	assert(sptr_get_dtor(&res[2]) == NULL);
	assert(sptr_get_dtor(&res[3]) == dtor);
	assert(sptr_get_dtor(&res[4]) == dtor);
	assert(sptr_get_dtor(&res[5]) == dtor);
}
NO_COV void test_sptr_make_shared_from_weak(void) {
	test_sptr_make_shared_from_weak_errors();
	test_sptr_make_shared_from_weak_ok();
}

static void test_sptr_make_shared_from_strong_errors(void) {
	void  *data     = (void *)0xDEAD;
	t_dtor dtor     = local_dtor;
	t_dtor not_dtor = other_dtor;

	t_fbox b0 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b1 = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 9};
	t_fbox b2 = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};
	t_fbox b3 = b0;
	b3.data   = &b3;

	t_strong_ptr s1_dt_as_neq     = (t_strong_ptr){b0.data, &b0};
	t_strong_ptr s2_data_meta_neq = (t_strong_ptr){(void *)(b0.data + 12), &b0};
	t_strong_ptr s3_data_null     = (t_strong_ptr){NULL, &b0};
	t_strong_ptr s4_meta_none     = (t_strong_ptr){b0.data, NULL};
	t_strong_ptr s5_data_meta_none = (t_strong_ptr){NULL, NULL};
	t_strong_ptr s6_data_is_meta   = (t_strong_ptr){&b3, &b3};
	t_strong_ptr s7_no_c           = (t_strong_ptr){&b2.data, &b2};
	t_strong_ptr s8_no_rc          = (t_strong_ptr){&b1.data, &b1};

	t_strong_ptr res[] = {
	    sptr_make_shared_from_strong(&s1_dt_as_neq, not_dtor),
	    sptr_make_shared_from_strong(&s2_data_meta_neq, not_dtor),
	    sptr_make_shared_from_strong(&s3_data_null, not_dtor),
	    sptr_make_shared_from_strong(&s4_meta_none, not_dtor),
	    sptr_make_shared_from_strong(&s5_data_meta_none, not_dtor),
	    sptr_make_shared_from_strong(&s6_data_is_meta, not_dtor),
	    sptr_make_shared_from_strong(&s7_no_c, not_dtor),
	    sptr_make_shared_from_strong(&s8_no_rc, not_dtor),
	    sptr_make_shared_from_strong(NULL, dtor),
	    sptr_make_shared_from_strong(NULL, NULL)};

	// note : leave assert unrolled, faster to see which failed
	assert(!res[0].data && !res[0].meta);
	assert(!res[1].data && !res[1].meta);
	assert(!res[2].data && !res[2].meta);
	assert(!res[3].data && !res[3].meta);
	assert(!res[4].data && !res[4].meta);
	assert(!res[5].data && !res[5].meta);
	assert(!res[6].data && !res[6].meta);
	assert(!res[7].data && !res[7].meta);
	assert(!res[8].data && !res[8].meta);
	assert(!res[9].data && !res[9].meta);
}
static void test_sptr_make_shared_from_strong_ok(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 0};
	t_fbox b1 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b2 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 0};
	t_fbox b3 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b4 = (t_fbox){.data = data, .dtor = dtor, .rc = 99, .wc = 0};
	t_fbox b5 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 99};

	// dt = dtor a=arg, s=source, neq=not equal, none=null;
	t_strong_ptr s0_dt_override_null = (t_strong_ptr){b0.data, &b0};
	t_strong_ptr s1_dt_left_as_is    = (t_strong_ptr){b1.data, &b1};
	t_strong_ptr s2_dt_left_as_null  = (t_strong_ptr){b2.data, &b2};
	t_strong_ptr s3_dt_match         = (t_strong_ptr){b3.data, &b3};
	t_strong_ptr s4_rc_many          = (t_strong_ptr){b4.data, &b4};
	t_strong_ptr s5_wc_many          = (t_strong_ptr){b5.data, &b5};

	t_strong_ptr res[] = {
	    sptr_make_shared_from_strong(&s0_dt_override_null, dtor),
	    sptr_make_shared_from_strong(&s1_dt_left_as_is, NULL),
	    sptr_make_shared_from_strong(&s2_dt_left_as_null, NULL),
	    sptr_make_shared_from_strong(&s3_dt_match, dtor),
	    sptr_make_shared_from_strong(&s4_rc_many, NULL),
	    sptr_make_shared_from_strong(&s5_wc_many, NULL),
	};

	assert(res[0].data != NULL && res[0].meta != NULL);
	assert(res[1].data != NULL && res[1].meta != NULL);
	assert(res[2].data != NULL && res[2].meta != NULL);
	assert(res[3].data != NULL && res[3].meta != NULL);
	assert(res[4].data != NULL && res[4].meta != NULL);
	assert(res[5].data != NULL && res[5].meta != NULL);

	assert(sptr_get_rc(&res[0]) == 2 && sptr_get_wc(&res[0]) == 0);
	assert(sptr_get_rc(&res[1]) == 2 && sptr_get_wc(&res[1]) == 0);
	assert(sptr_get_rc(&res[2]) == 2 && sptr_get_wc(&res[2]) == 0);
	assert(sptr_get_rc(&res[3]) == 2 && sptr_get_wc(&res[3]) == 0);
	assert(sptr_get_rc(&res[4]) == 100 && sptr_get_wc(&res[4]) == 0);
	assert(sptr_get_rc(&res[5]) == 2 && sptr_get_wc(&res[5]) == 99);

	assert(sptr_get(&res[0]) == data && sptr_get_dtor(&res[0]) == dtor);
	assert(sptr_get(&res[1]) == data && sptr_get_dtor(&res[1]) == dtor);
	assert(sptr_get(&res[2]) == data && sptr_get_dtor(&res[2]) == NULL);
	assert(sptr_get(&res[3]) == data && sptr_get_dtor(&res[3]) == dtor);
	assert(sptr_get(&res[4]) == data && sptr_get_dtor(&res[4]) == dtor);
	assert(sptr_get(&res[5]) == data && sptr_get_dtor(&res[5]) == dtor);
}
NO_COV void test_sptr_make_shared_from_strong(void) {
	test_sptr_make_shared_from_strong_errors();
	test_sptr_make_shared_from_strong_ok();
}

static void test_sptr_make_shared_from_owned_error(void) {
	void  *data     = (void *)0xDEAD;
	t_dtor dtor     = local_dtor;
	t_dtor not_dtor = other_dtor;

	t_fbox b0 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b1 = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};
	t_fbox b2 = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_fbox b3 = (t_fbox){.data = data, .dtor = dtor, .rc = 99, .wc = 0};
	t_fbox b4 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b5 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b6 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b7 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};

	t_owned_ptr no_own    = (t_owned_ptr){b0.data, &b0, false};
	t_owned_ptr no_c      = (t_owned_ptr){b1.data, &b1, true};
	t_owned_ptr no_rc     = (t_owned_ptr){b2.data, &b2, true};
	t_owned_ptr many_rc   = (t_owned_ptr){b3.data, &b3, true};
	t_owned_ptr data_null = (t_owned_ptr){NULL, &b4, true};
	t_owned_ptr data_diff = (t_owned_ptr){(void *)(b5.data + 12), &b5, true};
	t_owned_ptr dtor_mismatch = (t_owned_ptr){b6.data, &b6, true};
	t_owned_ptr no_meta       = (t_owned_ptr){b7.data, NULL, true};

	t_strong_ptr res[] = {
	    sptr_make_shared_from_owned(&no_own, NULL),
	    sptr_make_shared_from_owned(&no_c, NULL),
	    sptr_make_shared_from_owned(&no_rc, NULL),
	    sptr_make_shared_from_owned(&many_rc, NULL),
	    sptr_make_shared_from_owned(&data_null, NULL),
	    sptr_make_shared_from_owned(&data_diff, NULL),
	    sptr_make_shared_from_owned(&dtor_mismatch, not_dtor),
	    sptr_make_shared_from_owned(&no_meta, NULL),
	    sptr_make_shared_from_owned(NULL, NULL),
	};

	assert(!res[0].data && !res[0].meta);
	assert(!res[1].data && !res[1].meta);
	assert(!res[2].data && !res[2].meta);
	assert(!res[3].data && !res[3].meta);
	assert(!res[4].data && !res[4].meta);
	assert(!res[5].data && !res[5].meta);
	assert(!res[6].data && !res[6].meta);
	assert(!res[7].data && !res[7].meta);
	assert(!res[8].data && !res[8].meta);
	// TODO: assert that box fields did not move/change
}
static void test_sptr_make_shared_from_owned_ok(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b1 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 99};
	t_fbox b2 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 0};
	t_fbox b3 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b4 = (t_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 0};
	t_fbox b5 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};

	t_owned_ptr o0_wc_none     = (t_owned_ptr){b0.data, &b0, true};
	t_owned_ptr o1_wc_many     = (t_owned_ptr){b1.data, &b1, true};
	t_owned_ptr o2_dt_none     = (t_owned_ptr){b2.data, &b2, true};
	t_owned_ptr o3_dt_s        = (t_owned_ptr){b3.data, &b3, true};
	t_owned_ptr o4_dt_a        = (t_owned_ptr){b4.data, &b4, true};
	t_owned_ptr o5_dt_as_match = (t_owned_ptr){b5.data, &b5, true};

	t_strong_ptr res[] = {
	    sptr_make_shared_from_owned(&o0_wc_none, NULL),
	    sptr_make_shared_from_owned(&o1_wc_many, NULL),
	    sptr_make_shared_from_owned(&o2_dt_none, NULL),
	    sptr_make_shared_from_owned(&o3_dt_s, NULL),
	    sptr_make_shared_from_owned(&o4_dt_a, dtor),
	    sptr_make_shared_from_owned(&o5_dt_as_match, dtor),
	};

	assert(res[0].data != NULL && res[0].meta != NULL);
	assert(res[1].data != NULL && res[1].meta != NULL);
	assert(res[2].data != NULL && res[2].meta != NULL);
	assert(res[3].data != NULL && res[3].meta != NULL);
	assert(res[4].data != NULL && res[4].meta != NULL);
	assert(res[5].data != NULL && res[5].meta != NULL);
	assert(!o0_wc_none.data && !o0_wc_none.meta && !o0_wc_none.owner);
	assert(!o1_wc_many.data && !o1_wc_many.meta && !o1_wc_many.owner);
	assert(!o2_dt_none.data && !o2_dt_none.meta && !o2_dt_none.owner);
	assert(!o3_dt_s.data && !o3_dt_s.meta && !o3_dt_s.owner);
	assert(!o4_dt_a.data && !o4_dt_a.meta && !o4_dt_a.owner);
	assert(!o5_dt_as_match.data && !o5_dt_as_match.meta &&
	       !o5_dt_as_match.owner);

	// box re-used checks
	assert(res[0].meta == &b0);
	assert(res[1].meta == &b1);
	assert(res[2].meta == &b2);
	assert(res[3].meta == &b3);
	assert(res[4].meta == &b4);
	assert(res[5].meta == &b5);

	assert(sptr_get_rc(&res[0]) == 1 && sptr_get_wc(&res[0]) == 0);
	assert(sptr_get_rc(&res[1]) == 1 && sptr_get_wc(&res[1]) == 99);
	assert(sptr_get_rc(&res[2]) == 1 && sptr_get_wc(&res[2]) == 0);
	assert(sptr_get_rc(&res[3]) == 1 && sptr_get_wc(&res[3]) == 0);
	assert(sptr_get_rc(&res[4]) == 1 && sptr_get_wc(&res[4]) == 0);
	assert(sptr_get_rc(&res[5]) == 1 && sptr_get_wc(&res[5]) == 0);

	assert(sptr_get(&res[0]) == data && sptr_get_dtor(&res[0]) == dtor);
	assert(sptr_get(&res[1]) == data && sptr_get_dtor(&res[1]) == dtor);
	assert(sptr_get(&res[2]) == data && sptr_get_dtor(&res[2]) == NULL);
	assert(sptr_get(&res[3]) == data && sptr_get_dtor(&res[3]) == dtor);
	assert(sptr_get(&res[4]) == data && sptr_get_dtor(&res[4]) == dtor);
	assert(sptr_get(&res[5]) == data && sptr_get_dtor(&res[5]) == dtor);
}
NO_COV void test_sptr_make_shared_from_owned(void) {
	test_sptr_make_shared_from_owned_error();
	test_sptr_make_shared_from_owned_ok();
}
static void test_sptr_make_shared_from_uniq_error(void) {
	void  *data     = (void *)0xDEAD;
	t_dtor dtor     = local_dtor;
	t_dtor not_dtor = other_dtor;

	t_uniq_ptr u0 = (t_uniq_ptr){.data = data, .dtor = dtor};

	t_strong_ptr res[] = {
	    sptr_make_shared_from_uniq(&u0, not_dtor),
	    sptr_make_shared_from_uniq(NULL, NULL),
	    sptr_make_shared_from_uniq(NULL, dtor),
	    (t_strong_ptr){0},
	};

	alloc_set_call_limit(0);
	res[3] = sptr_make_shared_from_uniq(&u0, dtor);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(!res[0].data && !res[0].meta);
	assert(!res[1].data && !res[1].meta);
	assert(!res[2].data && !res[2].meta);
	assert(!res[3].data && !res[3].meta);
}
static void test_sptr_make_shared_from_uniq_ok(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_uniq_ptr u0 = (t_uniq_ptr){.data = data, .dtor = dtor}; // left
	t_uniq_ptr u1 = (t_uniq_ptr){.data = data, .dtor = dtor}; // match
	t_uniq_ptr u2 = (t_uniq_ptr){.data = data, .dtor = NULL}; // override
	t_uniq_ptr u3 = (t_uniq_ptr){.data = data, .dtor = NULL}; // left-null

	t_strong_ptr res[] = {
	    sptr_make_shared_from_uniq(&u0, NULL),
	    sptr_make_shared_from_uniq(&u1, dtor),
	    sptr_make_shared_from_uniq(&u2, dtor),
	    sptr_make_shared_from_uniq(&u3, NULL),
	};

	assert(res[0].data != NULL && res[0].meta != NULL); // ret is not null
	assert(res[1].data != NULL && res[1].meta != NULL);
	assert(res[2].data != NULL && res[2].meta != NULL);
	assert(res[3].data != NULL && res[3].meta != NULL);
	assert(!u0.data && !u0.dtor); // src was nulled
	assert(!u1.data && !u1.dtor);
	assert(!u2.data && !u2.dtor);
	assert(!u3.data && !u3.dtor);

	assert(sptr_get_rc(&res[0]) == 1 && sptr_get_wc(&res[0]) == 0);
	assert(sptr_get_rc(&res[1]) == 1 && sptr_get_wc(&res[1]) == 0);
	assert(sptr_get_rc(&res[2]) == 1 && sptr_get_wc(&res[2]) == 0);
	assert(sptr_get_rc(&res[3]) == 1 && sptr_get_wc(&res[3]) == 0);

	assert(sptr_get(&res[0]) == data && sptr_get_dtor(&res[0]) == dtor);
	assert(sptr_get(&res[1]) == data && sptr_get_dtor(&res[1]) == dtor);
	assert(sptr_get(&res[2]) == data && sptr_get_dtor(&res[2]) == dtor);
	assert(sptr_get(&res[3]) == data && sptr_get_dtor(&res[3]) == NULL);
	free(res[0].meta);
	free(res[1].meta);
	free(res[2].meta);
	free(res[3].meta);
}
NO_COV void test_sptr_make_shared_from_uniq(void) {
	test_sptr_make_shared_from_uniq_error();
	test_sptr_make_shared_from_uniq_ok();
}

// make weak

NO_COV void test_sptr_make_weak_from_weak(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_rw_ok   = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b1_rc_none = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_fbox b2_rc_many = (t_fbox){.data = data, .dtor = dtor, .rc = 99, .wc = 1};
	t_fbox b3_wc_many = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 99};
	t_fbox b4_wc_none = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};

	t_weak_ptr w0_rw_ok     = (t_weak_ptr){.meta = &b0_rw_ok};
	t_weak_ptr w1_rc_none   = (t_weak_ptr){.meta = &b1_rc_none};
	t_weak_ptr w2_rc_many   = (t_weak_ptr){.meta = &b2_rc_many};
	t_weak_ptr w3_wc_many   = (t_weak_ptr){.meta = &b3_wc_many};
	t_weak_ptr w4_wc_none   = (t_weak_ptr){.meta = &b4_wc_none};
	t_weak_ptr w5_meta_none = (t_weak_ptr){.meta = NULL};

	t_weak_ptr res[] = {sptr_make_weak_from_weak(&w0_rw_ok),
	                    sptr_make_weak_from_weak(&w1_rc_none),
	                    sptr_make_weak_from_weak(&w2_rc_many),
	                    sptr_make_weak_from_weak(&w3_wc_many),
	                    sptr_make_weak_from_weak(&w4_wc_none),
	                    sptr_make_weak_from_weak(&w5_meta_none),
	                    sptr_make_weak_from_weak(NULL)};
	assert(res[0].meta != NULL && res[0].meta == &b0_rw_ok);
	assert(res[1].meta != NULL && res[1].meta == &b1_rc_none);
	assert(res[2].meta != NULL && res[2].meta == &b2_rc_many);
	assert(res[3].meta != NULL && res[3].meta == &b3_wc_many);
	assert(!res[4].meta);
	assert(!res[5].meta);
	assert(!res[6].meta);

	assert(sptr_get_rc(&res[0]) == 1 && sptr_get_wc(&res[0]) == 2);
	assert(sptr_get_rc(&res[1]) == 0 && sptr_get_wc(&res[1]) == 2);
	assert(sptr_get_rc(&res[2]) == 99 && sptr_get_wc(&res[2]) == 2);
	assert(sptr_get_rc(&res[3]) == 1 && sptr_get_wc(&res[3]) == 100);

	// res[1] direct check due to rc==0 so "_get.*" protects access
	assert(sptr_get(&res[0]) == data && sptr_get_dtor(&res[0]) == dtor);
	assert(res[1].meta->data == data && res[1].meta->dtor == dtor);
	assert(sptr_get(&res[2]) == data && sptr_get_dtor(&res[2]) == dtor);
	assert(sptr_get(&res[3]) == data && sptr_get_dtor(&res[3]) == dtor);
}

NO_COV void test_sptr_make_weak_from_strong(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_rw_ok   = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b1_wc_none = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b2_rc_many = (t_fbox){.data = data, .dtor = dtor, .rc = 99, .wc = 1};
	t_fbox b3_wc_many = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 99};
	t_fbox b4_rc_none = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_fbox b6_ok      = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b7_ok      = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};

	t_strong_ptr s0_rw_ok   = (t_strong_ptr){.data = data, .meta = &b0_rw_ok};
	t_strong_ptr s1_wc_none = (t_strong_ptr){.data = data, .meta = &b1_wc_none};
	t_strong_ptr s2_rc_many = (t_strong_ptr){.data = data, .meta = &b2_rc_many};
	t_strong_ptr s3_wc_many = (t_strong_ptr){.data = data, .meta = &b3_wc_many};
	t_strong_ptr s4_rc_none = (t_strong_ptr){.data = data, .meta = &b4_rc_none};
	t_strong_ptr s5_data_diff = (t_strong_ptr){data + 12, .meta = &b7_ok};
	t_strong_ptr s6_data_none = (t_strong_ptr){NULL, .meta = &b6_ok};
	t_strong_ptr s7_meta_none = (t_strong_ptr){data, .meta = NULL};

	t_weak_ptr res[] = {sptr_make_weak_from_strong(&s0_rw_ok),
	                    sptr_make_weak_from_strong(&s1_wc_none),
	                    sptr_make_weak_from_strong(&s2_rc_many),
	                    sptr_make_weak_from_strong(&s3_wc_many),
	                    sptr_make_weak_from_strong(&s4_rc_none),
	                    sptr_make_weak_from_strong(&s5_data_diff),
	                    sptr_make_weak_from_strong(&s6_data_none),
	                    sptr_make_weak_from_strong(&s7_meta_none),
	                    sptr_make_weak_from_strong(NULL)};
	assert(res[0].meta != NULL && res[0].meta == &b0_rw_ok);
	assert(res[1].meta != NULL && res[1].meta == &b1_wc_none);
	assert(res[2].meta != NULL && res[2].meta == &b2_rc_many);
	assert(res[3].meta != NULL && res[3].meta == &b3_wc_many);
	assert(!res[4].meta);
	assert(!res[5].meta);
	assert(!res[6].meta);
	assert(!res[7].meta);
	assert(!res[8].meta);

	assert(sptr_get_rc(&res[0]) == 1 && sptr_get_wc(&res[0]) == 2);
	assert(sptr_get_rc(&res[1]) == 1 && sptr_get_wc(&res[1]) == 1);
	assert(sptr_get_rc(&res[2]) == 99 && sptr_get_wc(&res[2]) == 2);
	assert(sptr_get_rc(&res[3]) == 1 && sptr_get_wc(&res[3]) == 100);

	assert(sptr_get(&res[0]) == data && sptr_get_dtor(&res[0]) == dtor);
	assert(sptr_get(&res[1]) == data && sptr_get_dtor(&res[1]) == dtor);
	assert(sptr_get(&res[2]) == data && sptr_get_dtor(&res[2]) == dtor);
	assert(sptr_get(&res[3]) == data && sptr_get_dtor(&res[3]) == dtor);
}
