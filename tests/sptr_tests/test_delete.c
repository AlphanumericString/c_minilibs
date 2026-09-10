#include <assert.h>
#include <stdlib.h>

#include "sptr_tests.h"

static void bad(void *a) {
	(void)a;
	return;
}

void test_sptr_deletes(void) {
	t_strong_ptr s1 = sptr_make_shared("randcom string", NULL);
	t_strong_ptr s2 = sptr_make_shared(malloc(32), free);
	t_strong_ptr s3 = sptr_make_shared(malloc(32), free);
	t_weak_ptr   w1 = sptr_make_shared(s3, NULL);

	sptr_deletes(s1); // check via valgrind that mem was free'd
	sptr_deletes(s2);
	sptr_deletes(s3);
	assert(w1->rcb.data == NULL);
	sptr_deletew(w1);
	return;
}
void test_sptr_deletew(void) {
	t_strong_ptr s1 = sptr_make_shared(malloc(12), free);
	t_weak_ptr   w1 = sptr_make_shared(s1, bad);
	t_weak_ptr   w2 = sptr_make_shared(s1, NULL);
	t_weak_ptr   w3 = sptr_make_shared(s1, NULL);

	assert(!w1);
	sptr_deletew(w1);
	assert(sptr_get_wc(s1) == 2);
	sptr_deletew(w2);
	assert(sptr_get_wc(s1) == 1);
	sptr_deletew(w3);
	assert(sptr_get_wc(s1) == 0 && sptr_get_rc(s1) == 1);
	sptr_deletes(s1);
	return;
}
void test_sptr_deleteu(void) {
	t_uniq_ptr u1_owner    = sptr_make_uniq("random", NULL);
	t_uniq_ptr u1_borrower = sptr_make_uniq(u1_owner, NULL);
	t_uniq_ptr u2_owner    = sptr_make_uniq(malloc(4012), free);
	t_uniq_ptr u2_borrower = sptr_make_uniq(u2_owner, NULL);

	assert(u1_owner.owner == true && u2_owner.owner == true);
	assert(sptr_get_rc(u1_borrower) == 2);
	assert(sptr_get_rc(u2_borrower) == 2);
	sptr_deleteu(u1_owner);
	assert(sptr_get_rc(u1_borrower) == 1);
	sptr_deleteu(u1_borrower);
	sptr_deleteu(u2_borrower);
	assert(sptr_get_rc(u2_owner) == 1);
	sptr_deleteu(u2_owner);
	return;
}
