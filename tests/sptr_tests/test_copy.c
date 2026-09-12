#include <assert.h>
#include <stdlib.h>

#include "sptr_tests.h"

void test_sptr_copys(void) {
	t_strong_ptr *p1 = sptr_make_shared((void *)0xDEAD, NULL);
	t_strong_ptr *p2 = sptr_copys(p1);
	// TODO: error cases

	assert(p1 == p2);
	assert(p1->rc == 2);
	free(p1);
	return;
}
void test_sptr_copyw(void) {
	t_weak_ptr *w1 = sptr_weaken(sptr_make_shared((void *)0xDEAD, NULL));
	t_weak_ptr *w2 = sptr_copyw(w1);
	// TODO: error cases

	assert(w1 == w2);
	assert(w1->weak_count == 2);
	free(w1);
	return;
}
void test_sptr_copyu(void) {
	t_uniq_ptr u1 = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr u2 = sptr_copyu(u1);
	// TODO: error cases

	assert(u1.wcbox == u2.wcbox);
	assert(u1.owner == true && u2.owner == false);
	assert(u1.wcbox->rcb.rc == 2);
	free(u1.wcbox);
	return;
}
