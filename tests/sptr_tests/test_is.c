#include <assert.h>
#include <sptr/smart_pointer.h>

#include "sptr_tests.h"

// is_uniq
void test_sptr_is_uniqs(void) {
	t_strong_ptr s1 = sptr_make_shared((void *)0xDEAD, NULL);
	t_strong_ptr s2 = sptr_make_shared((void *)0xCAFE, NULL);
	t_strong_ptr s3;
	t_weak_ptr   w1 = sptr_make_shared(s2, NULL);

	assert(sptr_is_uniqs(s1) == true);
	assert(sptr_is_uniqs(s2) == true); // do weak counts?
	s3 = sptr_strengthen(w1);          // w1 invalidated
	assert(sptr_is_uniqs(s2) == false);
	assert(sptr_is_uniqs(NULL) == false);
	sptr_delete(s1);
	sptr_delete(s2);
	sptr_delete(s3);
	return;
}
void test_sptr_is_uniqw(void) {
	t_strong_ptr s1           = sptr_make_shared((void *)0xDEAD, NULL);
	t_strong_ptr s2           = sptr_make_shared((void *)0xCAFE, NULL);
	t_strong_ptr s3           = sptr_make_shared((void *)0xBEEF, NULL);
	t_strong_ptr s3_secondary = sptr_strengthen(sptr_make_shared(s3, NULL));

	t_weak_ptr w1 = sptr_make_shared(s1, NULL);
	t_weak_ptr w2 = sptr_make_shared(s2, NULL);
	t_weak_ptr w3 = sptr_make_shared(w2, NULL);
	t_weak_ptr w4 = sptr_make_shared(s3, NULL);

	sptr_delete(s1); // rc=0
	assert(sptr_is_uniqw(w1) == false);
	assert(sptr_is_uniqw(w2) == true);
	assert(sptr_is_uniqw(w3) == true);
	assert(sptr_is_uniqw(w4) == false);
	assert(sptr_is_uniqw(NULL) == false);
	sptr_delete(w1);
	sptr_delete(w2);
	sptr_delete(w3);
	sptr_delete(w4);
	sptr_delete(s2);
	sptr_delete(s3);
	sptr_delete(s3_secondary);
	return;
}
void test_sptr_is_uniqu(void) {
	t_uniq_ptr u1           = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr u2           = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr u2_secondary = sptr_make_uniq(u2, NULL);

	assert(sptr_is_uniqu(u1) == true);
	assert(sptr_is_uniqu(u2) == false);
	assert(sptr_is_uniqu(u2_secondary) == false);
	assert(sptr_is_uniqu((t_uniq_ptr){.rcbox = NULL, .owner = true}) == false);
	sptr_delete(u1);
	sptr_delete(u2);
	sptr_delete(u2_secondary);
	return;
}

// is_shared
void test_sptr_is_shareds(void) {
	t_strong_ptr s1 = sptr_make_shared((void *)0xDEAD, NULL);
	t_strong_ptr s2 = sptr_make_shared((void *)0xCAFE, NULL);
	t_strong_ptr s3;
	t_weak_ptr   w1 = sptr_make_shared(s2, NULL);

	assert(sptr_is_shareds(s1) == false);
	assert(sptr_is_shareds(s2) == false); // do weak counts?
	s3 = sptr_strengthen(w1);             // w1 invalidated
	assert(sptr_is_shareds(s2) == true);
	assert(sptr_is_shareds(NULL) == false);
	sptr_delete(s1);
	sptr_delete(s2);
	sptr_delete(s3);
	return;
}
void test_sptr_is_sharedw(void) {
	t_strong_ptr s1           = sptr_make_shared((void *)0xDEAD, NULL);
	t_strong_ptr s2           = sptr_make_shared((void *)0xCAFE, NULL);
	t_strong_ptr s2_secondary = sptr_strengthen(sptr_make_shared(s2, NULL));
	t_strong_ptr s3           = sptr_make_shared((void *)0xBEEF, NULL);

	// s1 : rc = 0, s2: rc = 2, s3: rc = 1
	t_weak_ptr w1 = sptr_make_shared(s1, NULL);
	t_weak_ptr w2 = sptr_make_shared(s2, NULL);
	t_weak_ptr w2_secondary;
	t_weak_ptr w3 = sptr_make_shared(s3, NULL);

	sptr_delete(s1); // rc=0
	assert(sptr_is_sharedw(w1) == false);
	assert(sptr_is_sharedw(w2) == true);
	w2_secondary = sptr_make_shared(w2, NULL);
	assert(sptr_is_sharedw(w2_secondary) == true); // check that weak dont count
	assert(sptr_is_sharedw(w3) == false);
	assert(sptr_is_sharedw(NULL) == false);
	sptr_delete(w1);
	sptr_delete(w2);
	sptr_delete(w2_secondary);
	sptr_delete(w3);
	sptr_delete(s2);
	sptr_delete(s2_secondary);
	sptr_delete(s3);
	return;
}
void test_sptr_is_sharedu(void) {
	t_uniq_ptr u1           = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr u2           = sptr_make_uniq((void *)0xCAFE, NULL);
	t_uniq_ptr u2_secondary = sptr_make_uniq(u2, NULL);

	assert(sptr_is_sharedu(u1) == false);
	assert(sptr_is_sharedu(u2) == true);
	assert(sptr_is_sharedu(u2_secondary) == true);
	assert(sptr_is_sharedu((t_uniq_ptr){0}) == false);
	assert(sptr_is_sharedu((t_uniq_ptr){.owner = true, .rcbox = NULL}) ==
	       false);
	sptr_delete(u1);
	sptr_delete(u2);
	sptr_delete(u2_secondary);
	return;
}

// is_own
void test_sptr_is_owns(void) {
	t_strong_ptr s1           = sptr_make_shared((void *)0xDEAD, NULL);
	t_strong_ptr s2           = sptr_make_shared((void *)0xCAFE, NULL);
	t_strong_ptr s2_secondary = sptr_copys(s2);

	assert(sptr_is_owns(s1) == true);
	assert(sptr_is_owns(s2) == false);
	assert(sptr_is_owns(NULL) == false);
	sptr_delete(s2_secondary);
	sptr_delete(s1);
	sptr_delete(s2);
	return;
}
void test_sptr_is_ownw(void) {
	t_strong_ptr s1           = sptr_make_shared((void *)0xDEAD, NULL);
	t_strong_ptr s2           = sptr_make_shared((void *)0xCAFE, NULL);
	t_strong_ptr s2_secondary = sptr_copys(s2);
	t_strong_ptr s3           = sptr_make_shared((void *)0xBEEF, NULL);

	// s1->rc=1, s2->rc=2, s3->rc=0
	t_weak_ptr w1 = sptr_make_shared(s1, NULL);
	t_weak_ptr w1_secondary;
	t_weak_ptr w2 = sptr_make_shared(s2, NULL);
	t_weak_ptr w3 = sptr_make_shared(s3, NULL);
	t_weak_ptr w3_secondary;

	sptr_delete(s3);
	assert(sptr_is_ownw(w1) == false);
	assert(sptr_is_ownw(w2) == false);
	assert(sptr_is_ownw(w3) == true);
	w1_secondary = sptr_copy(w1);
	w3_secondary = sptr_copy(w3);
	assert(sptr_is_ownw(w1) == false);
	assert(sptr_is_ownw(w3) == false);
	assert(sptr_is_ownw(NULL) == false);
	sptr_delete(w1);
	sptr_delete(w1_secondary);
	sptr_delete(w2);
	sptr_delete(w3);
	sptr_delete(w3_secondary);

	sptr_delete(s1);
	sptr_delete(s2);
	sptr_delete(s2_secondary);
	return;
}
void test_sptr_is_ownu(void) {
	t_uniq_ptr u1 = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr u2 = sptr_make_uniq((void *)0xCAFE, NULL);
	t_uniq_ptr u2_secondary;

	assert(sptr_is_ownu(u1) == true);
	assert(sptr_is_ownu(u2) == true);
	u2_secondary = sptr_copy(u2);
	assert(sptr_is_ownu(u2) == true);
	assert(sptr_is_ownu(u2_secondary) == false);
	assert(sptr_is_ownu((t_uniq_ptr){0}) == false);
	assert(sptr_is_ownu((t_uniq_ptr){.owner = true, .rcbox = NULL}) == false);
	sptr_delete(u1);
	sptr_delete(u2);
	sptr_delete(u2_secondary);
	return;
}
