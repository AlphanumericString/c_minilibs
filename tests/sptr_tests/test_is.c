#include <assert.h>

#include "sptr_tests.h"

static void local_dtor(void *_) {
	return;
}

// is_uniq
NO_COV void test_sptr_is_uniqs(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_rc1 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b1_rc0 = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};
	t_fbox b2_rc2 = (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 0};

	t_strong_ptr s0_rc1       = (t_strong_ptr){b0_rc1.data, &b0_rc1};
	t_strong_ptr s1_rc0       = (t_strong_ptr){b1_rc0.data, &b1_rc0};
	t_strong_ptr s2_rc2       = (t_strong_ptr){b2_rc2.data, &b2_rc2};
	t_strong_ptr s3_meta_null = (t_strong_ptr){data, NULL};

	bool res[] = {sptr_is_uniqs(&s0_rc1), sptr_is_uniqs(&s1_rc0),
	              sptr_is_uniqs(&s2_rc2), sptr_is_uniqs(&s3_meta_null),
	              sptr_is_uniqs(NULL)};

	assert(res[0] == true);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
}

NO_COV void test_sptr_is_uniqw(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_rc1 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b1_rc0 = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_fbox b2_rc2 = (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 1};

	t_weak_ptr w0_rc1       = (t_weak_ptr){&b0_rc1};
	t_weak_ptr w1_rc0       = (t_weak_ptr){&b1_rc0};
	t_weak_ptr w2_rc2       = (t_weak_ptr){&b2_rc2};
	t_weak_ptr w3_meta_null = (t_weak_ptr){NULL};

	bool res[] = {sptr_is_uniqw(&w0_rc1), sptr_is_uniqw(&w1_rc0),
	              sptr_is_uniqw(&w2_rc2), sptr_is_uniqw(&w3_meta_null),
	              sptr_is_uniqw(NULL)};

	assert(res[0] == true);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
}

NO_COV void test_sptr_is_uniqo(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_rc1 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b1_rc0 = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};
	t_fbox b2_rc2 = (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 0};

	t_owned_ptr o0_rc1       = (t_owned_ptr){b0_rc1.data, &b0_rc1, true};
	t_owned_ptr o1_rc0       = (t_owned_ptr){b1_rc0.data, &b1_rc0, true};
	t_owned_ptr o2_rc2       = (t_owned_ptr){b2_rc2.data, &b2_rc2, true};
	t_owned_ptr o3_meta_null = (t_owned_ptr){data, NULL, true};

	bool res[] = {sptr_is_uniqo(&o0_rc1), sptr_is_uniqo(&o1_rc0),
	              sptr_is_uniqo(&o2_rc2), sptr_is_uniqo(&o3_meta_null),
	              sptr_is_uniqo(NULL)};

	assert(res[0] == true);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
}

NO_COV void test_sptr_is_uniqu(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_uniq_ptr u0_ok        = (t_uniq_ptr){.data = data, .dtor = dtor};
	t_uniq_ptr u1_data_null = (t_uniq_ptr){.data = NULL, .dtor = dtor};

	bool res[] = {sptr_is_uniqu(&u0_ok), sptr_is_uniqu(&u1_data_null),
	              sptr_is_uniqu(NULL)};

	assert(res[0] == true);
	assert(!res[1]);
	assert(!res[2]);
}

// is_shared
NO_COV void test_sptr_is_shareds(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_rc0 = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};
	t_fbox b1_rc1 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b2_rc2 = (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 0};

	t_strong_ptr s0_rc0       = (t_strong_ptr){b0_rc0.data, &b0_rc0};
	t_strong_ptr s1_rc1       = (t_strong_ptr){b1_rc1.data, &b1_rc1};
	t_strong_ptr s2_rc2       = (t_strong_ptr){b2_rc2.data, &b2_rc2};
	t_strong_ptr s3_meta_null = (t_strong_ptr){data, NULL};

	bool res[] = {
	    sptr_is_shareds(&s0_rc0), sptr_is_shareds(&s1_rc1),
	    sptr_is_shareds(&s2_rc2), sptr_is_shareds(&s3_meta_null),
	    sptr_is_shareds(NULL),
	};

	assert(!res[0]);
	assert(res[1] == true);
	assert(res[2] == true);
	assert(!res[3]);
	assert(!res[4]);
}

NO_COV void test_sptr_is_sharedw(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_rc0 = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_fbox b1_rc1 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b2_rc2 = (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 1};

	t_weak_ptr w0_rc0       = (t_weak_ptr){&b0_rc0};
	t_weak_ptr w1_rc1       = (t_weak_ptr){&b1_rc1};
	t_weak_ptr w2_rc2       = (t_weak_ptr){&b2_rc2};
	t_weak_ptr w3_meta_null = (t_weak_ptr){NULL};

	bool res[] = {
	    sptr_is_sharedw(&w0_rc0), sptr_is_sharedw(&w1_rc1),
	    sptr_is_sharedw(&w2_rc2), sptr_is_sharedw(&w3_meta_null),
	    sptr_is_sharedw(NULL),
	};

	assert(!res[0]);
	assert(res[1] == true);
	assert(res[2] == true);
	assert(!res[3]);
	assert(!res[4]);
}

NO_COV void test_sptr_is_sharedo(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_rc0 = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};
	t_fbox b1_rc1 = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b2_rc2 = (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 0};

	t_owned_ptr o0_rc0       = (t_owned_ptr){b0_rc0.data, &b0_rc0, true};
	t_owned_ptr o1_rc1       = (t_owned_ptr){b1_rc1.data, &b1_rc1, true};
	t_owned_ptr o2_rc2       = (t_owned_ptr){b2_rc2.data, &b2_rc2, true};
	t_owned_ptr o3_meta_null = (t_owned_ptr){data, NULL, true};

	bool res[] = {
	    sptr_is_sharedo(&o0_rc0), sptr_is_sharedo(&o1_rc1),
	    sptr_is_sharedo(&o2_rc2), sptr_is_sharedo(&o3_meta_null),
	    sptr_is_sharedo(NULL),
	};

	assert(!res[0]);
	assert(res[1] == true);
	assert(res[2] == true);
	assert(!res[3]);
	assert(!res[4]);
}

NO_COV void test_sptr_is_sharedu(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_uniq_ptr u0_ok        = (t_uniq_ptr){.data = data, .dtor = dtor};
	t_uniq_ptr u1_data_null = (t_uniq_ptr){.data = NULL, .dtor = dtor};

	bool res[] = {
	    sptr_is_sharedu(&u0_ok),
	    sptr_is_sharedu(&u1_data_null),
	    sptr_is_sharedu(NULL),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
}

// is_sole_box_owner
NO_COV void test_sptr_is_sole_box_owns(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_sole    = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b1_wc      = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b2_rc_many = (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 0};
	t_fbox b3_no_rc   = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};

	t_strong_ptr s0_sole      = (t_strong_ptr){b0_sole.data, &b0_sole};
	t_strong_ptr s1_wc        = (t_strong_ptr){b1_wc.data, &b1_wc};
	t_strong_ptr s2_rc_many   = (t_strong_ptr){b2_rc_many.data, &b2_rc_many};
	t_strong_ptr s3_no_rc     = (t_strong_ptr){b3_no_rc.data, &b3_no_rc};
	t_strong_ptr s4_meta_null = (t_strong_ptr){data, NULL};

	bool res[] = {
	    sptri_is_sole_box_owns(&s0_sole),
	    sptri_is_sole_box_owns(&s1_wc),
	    sptri_is_sole_box_owns(&s2_rc_many),
	    sptri_is_sole_box_owns(&s3_no_rc),
	    sptri_is_sole_box_owns(&s4_meta_null),
	    sptri_is_sole_box_owns(NULL),
	};

	assert(res[0] == true);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
	assert(!res[5]);
}

NO_COV void test_sptr_is_sole_box_ownw(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_sole    = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_fbox b1_has_rc  = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b2_wc_many = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 2};
	t_fbox b3_no_wc   = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};

	t_weak_ptr w0_sole      = (t_weak_ptr){&b0_sole};
	t_weak_ptr w1_has_rc    = (t_weak_ptr){&b1_has_rc};
	t_weak_ptr w2_wc_many   = (t_weak_ptr){&b2_wc_many};
	t_weak_ptr w3_no_wc     = (t_weak_ptr){&b3_no_wc};
	t_weak_ptr w4_meta_null = (t_weak_ptr){NULL};

	bool res[] = {
	    sptri_is_sole_box_ownw(&w0_sole),
	    sptri_is_sole_box_ownw(&w1_has_rc),
	    sptri_is_sole_box_ownw(&w2_wc_many),
	    sptri_is_sole_box_ownw(&w3_no_wc),
	    sptri_is_sole_box_ownw(&w4_meta_null),
	    sptri_is_sole_box_ownw(NULL),
	};

	assert(res[0] == true);
	assert(!res[1]);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
	assert(!res[5]);
}

NO_COV void test_sptr_is_sole_box_owno(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_fbox b0_sole_rc = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 0};
	t_fbox b1_sole_wc = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 1};
	t_fbox b2_rc_wc   = (t_fbox){.data = data, .dtor = dtor, .rc = 1, .wc = 1};
	t_fbox b3_rc_many = (t_fbox){.data = data, .dtor = dtor, .rc = 2, .wc = 0};
	t_fbox b4_wc_many = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 2};
	t_fbox b5_no_c    = (t_fbox){.data = data, .dtor = dtor, .rc = 0, .wc = 0};

	t_owned_ptr o0_sole_rc = (t_owned_ptr){b0_sole_rc.data, &b0_sole_rc, true};
	t_owned_ptr o1_sole_wc = (t_owned_ptr){b1_sole_wc.data, &b1_sole_wc, false};
	t_owned_ptr o2_rc_wc   = (t_owned_ptr){b2_rc_wc.data, &b2_rc_wc, true};
	t_owned_ptr o3_rc_many = (t_owned_ptr){b3_rc_many.data, &b3_rc_many, true};
	t_owned_ptr o4_wc_many = (t_owned_ptr){b4_wc_many.data, &b4_wc_many, false};
	t_owned_ptr o5_no_c    = (t_owned_ptr){b5_no_c.data, &b5_no_c, false};
	t_owned_ptr o6_meta_null = (t_owned_ptr){data, NULL, true};

	bool res[] = {
	    sptri_is_sole_box_owno(&o0_sole_rc),
	    sptri_is_sole_box_owno(&o1_sole_wc),
	    sptri_is_sole_box_owno(&o2_rc_wc),
	    sptri_is_sole_box_owno(&o3_rc_many),
	    sptri_is_sole_box_owno(&o4_wc_many),
	    sptri_is_sole_box_owno(&o5_no_c),
	    sptri_is_sole_box_owno(&o6_meta_null),
	    sptri_is_sole_box_owno(NULL),
	};

	assert(res[0] == true);
	assert(res[1] == true);
	assert(!res[2]);
	assert(!res[3]);
	assert(!res[4]);
	assert(!res[5]);
	assert(!res[6]);
	assert(!res[7]);
}

NO_COV void test_sptr_is_sole_box_ownu(void) {
	void  *data = (void *)0xDEAD;
	t_dtor dtor = local_dtor;

	t_uniq_ptr u0_ok        = (t_uniq_ptr){.data = data, .dtor = dtor};
	t_uniq_ptr u1_data_null = (t_uniq_ptr){.data = NULL, .dtor = dtor};

	bool res[] = {
	    sptri_is_sole_box_ownu(&u0_ok),
	    sptri_is_sole_box_ownu(&u1_data_null),
	    sptri_is_sole_box_ownu(NULL),
	};

	assert(!res[0]);
	assert(!res[1]);
	assert(!res[2]);
}
