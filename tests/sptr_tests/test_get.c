#include <assert.h>
#include <sptr/smart_pointer.h>

#include "sptr_tests.h"

// const-data
void test_sptr_getsc(void) {
	t_strong_ptr s1 = sptr_make_shared((void *)0xDEAD, NULL);

	assert(sptr_getsc(s1) == (const void *)0xDEAD);
	assert(sptr_getsc(NULL) == NULL);
	sptr_delete(s1);
	return;
}
void test_sptr_getwc(void) {
	t_weak_ptr   w1 = sptr_weaken(sptr_make_shared((void *)0xDEAD, NULL));
	t_strong_ptr s1 = sptr_make_shared((void *)0xCAFE, NULL);
	t_weak_ptr   w2 = sptr_make_shared(s1, NULL);

	assert(sptr_getwc(w1) == NULL); // 0 rc -> NULL
	assert(sptr_getwc(w2) == (const void *)0xCAFE);
	assert(sptr_getwc(NULL) == NULL);
	sptr_delete(w1);
	sptr_delete(w2);
	sptr_delete(s1);
	return;
}
// data
void test_sptr_gets(void) {
	t_strong_ptr s1 = sptr_make_shared((void *)0xDEAD, NULL);

	assert(sptr_gets(s1) == (void *)0xDEAD);
	assert(sptr_gets(NULL) == NULL);
	sptr_delete(s1);
	return;
}
void test_sptr_getw(void) {
	t_weak_ptr   w1 = sptr_weaken(sptr_make_shared((void *)0xDEAD, NULL));
	t_strong_ptr s1 = sptr_make_shared((void *)0xCAFE, NULL);
	t_weak_ptr   w2 = sptr_make_shared(s1, NULL);

	assert(sptr_getw(w1) == NULL);
	assert(sptr_getw(w2) == (void *)0xCAFE);
	assert(sptr_getw(NULL) == NULL);
	sptr_delete(w1);
	sptr_delete(s1);
	assert(sptr_getw(w2) == NULL);
	sptr_delete(w2);
	return;
}
void test_sptr_getu(void) {
	t_uniq_ptr u1        = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr u2        = sptr_make_uniq((void *)0xCAFE, NULL);
	t_uniq_ptr u2_borrow = sptr_make_uniq(u2, NULL);

	assert(sptr_getu(u1) == (void *)0xDEAD);
	assert(sptr_getu(u2_borrow) == (void *)0xCAFE);
	assert(sptr_getu((t_uniq_ptr){.owner = true, .rcbox = NULL}) == NULL);
	sptr_delete(u1);
	sptr_delete(u2);
	sptr_delete(u2_borrow);
	return;
}

// ref-count
void test_sptr_get_rcs(void) {
	t_strong_ptr s1 = sptr_make_shared((void *)0xDEAD, NULL);

	assert(sptr_get_rcs(s1) == 1);
	assert(sptr_get_rcs(NULL) == 0);
	sptr_delete(s1);
	return;
}
void test_sptr_get_rcw(void) {
	t_weak_ptr   w1 = sptr_weaken(sptr_make_shared((void *)0xDEAD, NULL));
	t_strong_ptr s1 = sptr_make_shared((void *)0xCAFE, NULL);
	t_weak_ptr   w2 = sptr_make_shared(s1, NULL);

	assert(sptr_get_rcw(w1) == 0);
	assert(sptr_get_rcw(w2) == 1);
	assert(sptr_get_rcw(NULL) == 0);
	sptr_delete(w1);
	sptr_delete(w2);
	sptr_deletes(s1);
	return;
}
void test_sptr_get_rcu(void) {
	t_uniq_ptr u1           = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr u2           = sptr_make_uniq((void *)0xCAFE, NULL);
	t_uniq_ptr u2_secondary = sptr_make_uniq(u2, NULL);

	assert(sptr_get_rcu(u1) == 1);
	assert(sptr_get_rcu(u2) == 2);
	assert(sptr_get_rcu((t_uniq_ptr){.owner = true, .rcbox = NULL}) == 0);
	sptr_delete(u1);
	sptr_delete(u2);
	sptr_delete(u2_secondary);
	return;
}

// weak-count
void test_sptr_get_wcs(void) {
	t_strong_ptr s1 = sptr_make_shared((void *)0xDEAD, NULL);
	t_strong_ptr s2 = sptr_make_shared((void *)0xCAFE, NULL);
	t_weak_ptr   w1 = sptr_make_shared(s2, NULL);

	assert(sptr_get_wcs(s1) == 0);
	assert(sptr_get_wcs(s2) == 1);
	assert(sptr_get_wcs(NULL) == 0);
	sptr_delete(s1);
	sptr_delete(s2);
	sptr_delete(w1);
	return;
}
void test_sptr_get_wcw(void) {
	t_weak_ptr w1 = sptr_weaken(sptr_make_shared((void *)0xDEAD, NULL));
	t_weak_ptr w2 = sptr_weaken(sptr_make_shared((void *)0xCAFE, NULL));
	t_weak_ptr w2_secondary = sptr_make_shared(w2, NULL);

	assert(sptr_get_wcw(w1) == 1);
	assert(sptr_get_wcw(w2) == 2);
	assert(sptr_get_wcw(NULL) == 0);
	sptr_delete(w2_secondary);
	sptr_delete(w2);
	sptr_delete(w1);
	return;
}
void test_sptr_get_wcu(void) {
	t_uniq_ptr u1           = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr u2           = sptr_make_uniq((void *)0xCAFE, NULL);
	t_uniq_ptr u2_secondary = sptr_make_uniq(u2, NULL);

	assert(sptr_get_wcu(u1) == 0);
	assert(sptr_get_wcu(u2) == 1);
	assert(sptr_get_wcu((t_uniq_ptr){.owner = true, .rcbox = NULL}) == 0);
	sptr_delete(u1);
	sptr_delete(u2);
	sptr_delete(u2_secondary);
	return;
}
