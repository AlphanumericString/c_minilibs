#include <assert.h>
#include <stdlib.h>

#include "sptr_tests.h"

static void other_dtor(void *_) {
	return;
}
static void local_dtor(void *_) {
	return;
}

// make_uniq_from_raw
static void test_sptr_make_uniq_from_raw_errors(void) {
	t_cm_dtor dtor = local_dtor;

	t_cm_uniq_ptr res[] = {
	    sptr_make_uniq_from_raw(NULL, NULL),
	    sptr_make_uniq_from_raw(NULL, dtor),
	};

	assert(!res[0].data && !res[0].dtor);
	assert(!res[1].data && !res[1].dtor);
}

static void test_sptr_make_uniq_from_raw_ok(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_uniq_ptr res[] = {
	    sptr_make_uniq_from_raw(data, NULL),
	    sptr_make_uniq_from_raw(data, dtor),
	};

	assert(res[0].data == data && res[0].dtor == NULL);
	assert(res[1].data == data && res[1].dtor == dtor);
}

NO_COV void test_sptr_make_uniq_from_raw(void) {
	test_sptr_make_uniq_from_raw_errors();
	test_sptr_make_uniq_from_raw_ok();
}

// make_uniq_from_weak (always illegal)
NO_COV void test_sptr_make_uniq_from_weak(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_fbox b0 = (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_cm_fbox b1 = (t_cm_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};

	t_cm_weak_ptr w0 = (t_cm_weak_ptr){&b0};
	t_cm_weak_ptr w1 = (t_cm_weak_ptr){&b1};
	t_cm_weak_ptr w2 = (t_cm_weak_ptr){NULL};

	t_cm_uniq_ptr res[] = {
	    sptr_make_uniq_from_weak(&w0, NULL),
	    sptr_make_uniq_from_weak(&w0, dtor),
	    sptr_make_uniq_from_weak(&w1, NULL),
	    sptr_make_uniq_from_weak(&w2, NULL),
	    sptr_make_uniq_from_weak(NULL, NULL),
	    sptr_make_uniq_from_weak(NULL, dtor),
	};

	assert(!res[0].data && !res[0].dtor);
	assert(!res[1].data && !res[1].dtor);
	assert(!res[2].data && !res[2].dtor);
	assert(!res[3].data && !res[3].dtor);
	assert(!res[4].data && !res[4].dtor);
	assert(!res[5].data && !res[5].dtor);
}

// make_uniq_from_strong
static void test_sptr_make_uniq_from_strong_errors(void) {
	void     *data     = (void *)0xDEAD;
	t_cm_dtor dtor     = local_dtor;
	t_cm_dtor not_dtor = other_dtor;

	t_cm_fbox b0_no_rc =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_cm_fbox b1_rc_many =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 1};
	t_cm_fbox b2_data_null =
	    (t_cm_fbox){.data = NULL, .dtor = dtor, .rc = 1, .wc = 1};
	t_cm_fbox b3_dt_mismatch =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};

	t_cm_strong_ptr s0_no_rc = (t_cm_strong_ptr){b0_no_rc.data, &b0_no_rc};
	t_cm_strong_ptr s1_rc_many =
	    (t_cm_strong_ptr){b1_rc_many.data, &b1_rc_many};
	t_cm_strong_ptr s2_data_null = (t_cm_strong_ptr){NULL, &b2_data_null};
	t_cm_strong_ptr s3_dt_mismatch =
	    (t_cm_strong_ptr){b3_dt_mismatch.data, &b3_dt_mismatch};
	t_cm_strong_ptr s4_meta_null = (t_cm_strong_ptr){data, NULL};

	t_cm_uniq_ptr res[] = {
	    sptr_make_uniq_from_strong(NULL, NULL),
	    sptr_make_uniq_from_strong(NULL, dtor),
	    sptr_make_uniq_from_strong(&s0_no_rc, NULL),
	    sptr_make_uniq_from_strong(&s1_rc_many, NULL),
	    sptr_make_uniq_from_strong(&s2_data_null, NULL),
	    sptr_make_uniq_from_strong(&s3_dt_mismatch, not_dtor),
	    sptr_make_uniq_from_strong(&s4_meta_null, NULL),
	};

	assert(!res[0].data && !res[0].dtor);
	assert(!res[1].data && !res[1].dtor);
	assert(!res[2].data && !res[2].dtor);
	assert(!res[3].data && !res[3].dtor);
	assert(!res[4].data && !res[4].dtor);
	assert(!res[5].data && !res[5].dtor);
	assert(!res[6].data && !res[6].dtor);
}

static void test_sptr_make_uniq_from_strong_ok(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_fbox  b0 = (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_cm_fbox  b1 = (t_cm_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};
	t_cm_fbox  b2 = (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 2};
	t_cm_fbox  b3 = (t_cm_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};
	t_cm_fbox *b4 = malloc(sizeof(*b4));
	assert(b4);

	*b4 = (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};

	t_cm_strong_ptr s0_dt_match      = (t_cm_strong_ptr){b0.data, &b0};
	t_cm_strong_ptr s1_dt_override   = (t_cm_strong_ptr){b1.data, &b1};
	t_cm_strong_ptr s2_dt_left_as_is = (t_cm_strong_ptr){b2.data, &b2};
	t_cm_strong_ptr s3_dt_left_null  = (t_cm_strong_ptr){b3.data, &b3};
	t_cm_strong_ptr s4_wc_none       = (t_cm_strong_ptr){b4->data, b4};

	t_cm_uniq_ptr res[] = {
	    sptr_make_uniq_from_strong(&s0_dt_match, dtor),
	    sptr_make_uniq_from_strong(&s1_dt_override, dtor),
	    sptr_make_uniq_from_strong(&s2_dt_left_as_is, NULL),
	    sptr_make_uniq_from_strong(&s3_dt_left_null, NULL),
	    sptr_make_uniq_from_strong(&s4_wc_none, NULL),
	};

	assert(res[0].data == data && res[0].dtor == dtor);
	assert(res[1].data == data && res[1].dtor == dtor);
	assert(res[2].data == data && res[2].dtor == dtor);
	assert(res[3].data == data && res[3].dtor == NULL);
	assert(res[4].data == data && res[4].dtor == dtor);

	// Original strong pointers nulled out
	assert(!s0_dt_match.data && !s0_dt_match.meta);
	assert(!s1_dt_override.data && !s1_dt_override.meta);
	assert(!s2_dt_left_as_is.data && !s2_dt_left_as_is.meta);
	assert(!s3_dt_left_null.data && !s3_dt_left_null.meta);
	assert(!s4_wc_none.data && !s4_wc_none.meta);
	// b5 was freed by sptr_make_uniq_from_strong since wc is 0
}

NO_COV void test_sptr_make_uniq_from_strong(void) {
	test_sptr_make_uniq_from_strong_errors();
	test_sptr_make_uniq_from_strong_ok();
}

// make_uniq_from_owned
static void test_sptr_make_uniq_from_owned_errors(void) {
	void     *data     = (void *)0xDEAD;
	t_cm_dtor dtor     = local_dtor;
	t_cm_dtor not_dtor = other_dtor;

	t_cm_fbox b0_not_owner =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_cm_fbox b1_no_rc =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_cm_fbox b2_rc_many =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 1};
	t_cm_fbox b3_dt_mismatch =
	    (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_cm_fbox b4_data_null =
	    (t_cm_fbox){.data = NULL, .dtor = dtor, .rc = 1, .wc = 1};

	t_cm_owned_ptr o0_not_owner =
	    (t_cm_owned_ptr){b0_not_owner.data, &b0_not_owner, false};
	t_cm_owned_ptr o1_no_rc = (t_cm_owned_ptr){b1_no_rc.data, &b1_no_rc, true};
	t_cm_owned_ptr o2_rc_many =
	    (t_cm_owned_ptr){b2_rc_many.data, &b2_rc_many, true};
	t_cm_owned_ptr o3_dt_mismatch =
	    (t_cm_owned_ptr){b3_dt_mismatch.data, &b3_dt_mismatch, true};
	t_cm_owned_ptr o4_data_null = (t_cm_owned_ptr){NULL, &b4_data_null, true};
	t_cm_owned_ptr o5_meta_null = (t_cm_owned_ptr){data, NULL, true};

	t_cm_uniq_ptr res[] = {
	    sptr_make_uniq_from_owned(NULL, NULL),
	    sptr_make_uniq_from_owned(NULL, dtor),
	    sptr_make_uniq_from_owned(&o0_not_owner, NULL),
	    sptr_make_uniq_from_owned(&o1_no_rc, NULL),
	    sptr_make_uniq_from_owned(&o2_rc_many, NULL),
	    sptr_make_uniq_from_owned(&o3_dt_mismatch, not_dtor),
	    sptr_make_uniq_from_owned(&o4_data_null, NULL),
	    sptr_make_uniq_from_owned(&o5_meta_null, NULL),
	};

	assert(!res[0].data && !res[0].dtor);
	assert(!res[1].data && !res[1].dtor);
	assert(!res[2].data && !res[2].dtor);
	assert(!res[3].data && !res[3].dtor);
	assert(!res[4].data && !res[4].dtor);
	assert(!res[5].data && !res[5].dtor);
	assert(!res[6].data && !res[6].dtor);
	assert(!res[7].data && !res[7].dtor);
}

static void test_sptr_make_uniq_from_owned_ok(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_fbox b0 = (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_cm_fbox b1 = (t_cm_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};
	t_cm_fbox b2 = (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 2};
	t_cm_fbox b3 = (t_cm_fbox){.data = data, .dtor = NULL, .rc = 1, .wc = 1};

	// For wc == 0, sptr_make_uniq_from_owned calls free(meta)
	t_cm_fbox *heap_box = malloc(sizeof(*heap_box));
	assert(heap_box != NULL);
	*heap_box = (t_cm_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};

	t_cm_owned_ptr o0_dt_match      = (t_cm_owned_ptr){b0.data, &b0, true};
	t_cm_owned_ptr o1_dt_override   = (t_cm_owned_ptr){b1.data, &b1, true};
	t_cm_owned_ptr o2_dt_left_as_is = (t_cm_owned_ptr){b2.data, &b2, true};
	t_cm_owned_ptr o3_dt_left_null  = (t_cm_owned_ptr){b3.data, &b3, true};
	t_cm_owned_ptr o4_wc_none_heap =
	    (t_cm_owned_ptr){heap_box->data, heap_box, true};

	t_cm_uniq_ptr res[] = {
	    sptr_make_uniq_from_owned(&o0_dt_match, dtor),
	    sptr_make_uniq_from_owned(&o1_dt_override, dtor),
	    sptr_make_uniq_from_owned(&o2_dt_left_as_is, NULL),
	    sptr_make_uniq_from_owned(&o3_dt_left_null, NULL),
	    sptr_make_uniq_from_owned(&o4_wc_none_heap, NULL),
	};

	assert(res[0].data == data && res[0].dtor == dtor);
	assert(res[1].data == data && res[1].dtor == dtor);
	assert(res[2].data == data && res[2].dtor == dtor);
	assert(res[3].data == data && res[3].dtor == NULL);
	assert(res[4].data == data && res[4].dtor == dtor);

	// Original owned pointers nulled out
	assert(!o0_dt_match.data && !o0_dt_match.meta && !o0_dt_match.owner);
	assert(!o1_dt_override.data && !o1_dt_override.meta &&
	       !o1_dt_override.owner);
	assert(!o2_dt_left_as_is.data && !o2_dt_left_as_is.meta &&
	       !o2_dt_left_as_is.owner);
	assert(!o3_dt_left_null.data && !o3_dt_left_null.meta &&
	       !o3_dt_left_null.owner);
	assert(!o4_wc_none_heap.data && !o4_wc_none_heap.meta &&
	       !o4_wc_none_heap.owner);

	assert(b0.rc == 0);
	assert(b1.rc == 0);
	assert(b2.rc == 0);
	assert(b3.rc == 0);
	// heap_box was freed by sptr_make_uniq_from_owned
}

NO_COV void test_sptr_make_uniq_from_owned(void) {
	test_sptr_make_uniq_from_owned_errors();
	test_sptr_make_uniq_from_owned_ok();
}

// make_uniq_from_uniq
static void test_sptr_make_uniq_from_uniq_errors(void) {
	void     *data     = (void *)0xDEAD;
	t_cm_dtor dtor     = local_dtor;
	t_cm_dtor not_dtor = other_dtor;

	t_cm_uniq_ptr u0_data_null   = (t_cm_uniq_ptr){.data = NULL, .dtor = dtor};
	t_cm_uniq_ptr u1_dt_mismatch = (t_cm_uniq_ptr){.data = data, .dtor = dtor};

	t_cm_uniq_ptr res[] = {
	    sptr_make_uniq_from_uniq(NULL, NULL),
	    sptr_make_uniq_from_uniq(NULL, dtor),
	    sptr_make_uniq_from_uniq(&u0_data_null, NULL),
	    sptr_make_uniq_from_uniq(&u1_dt_mismatch, not_dtor),
	};

	assert(!res[0].data && !res[0].dtor);
	assert(!res[1].data && !res[1].dtor);
	assert(!res[2].data && !res[2].dtor);
	assert(!res[3].data && !res[3].dtor);
}

static void test_sptr_make_uniq_from_uniq_ok(void) {
	void     *data = (void *)0xDEAD;
	t_cm_dtor dtor = local_dtor;

	t_cm_uniq_ptr u0 = (t_cm_uniq_ptr){.data = data, .dtor = dtor};
	t_cm_uniq_ptr u1 = (t_cm_uniq_ptr){.data = data, .dtor = NULL};
	t_cm_uniq_ptr u2 = (t_cm_uniq_ptr){.data = data, .dtor = dtor};
	t_cm_uniq_ptr u3 = (t_cm_uniq_ptr){.data = data, .dtor = NULL};

	t_cm_uniq_ptr res[] = {
	    sptr_make_uniq_from_uniq(&u0, dtor),
	    sptr_make_uniq_from_uniq(&u1, dtor),
	    sptr_make_uniq_from_uniq(&u2, NULL),
	    sptr_make_uniq_from_uniq(&u3, NULL),
	};

	assert(res[0].data == data && res[0].dtor == dtor);
	assert(res[1].data == data && res[1].dtor == dtor);
	assert(res[2].data == data && res[2].dtor == dtor);
	assert(res[3].data == data && res[3].dtor == NULL);

	assert(!u0.data && !u0.dtor);
	assert(!u1.data && !u1.dtor);
	assert(!u2.data && !u2.dtor);
	assert(!u3.data && !u3.dtor);
}

NO_COV void test_sptr_make_uniq_from_uniq(void) {
	test_sptr_make_uniq_from_uniq_errors();
	test_sptr_make_uniq_from_uniq_ok();
}
