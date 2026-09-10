#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>

#include "sptr/smart_pointer.h"
#include "sptr_tests.h"

void test_sptr_strengthen(void) {
	t_strong_ptr src      = sptr_make_shared((void *)0xDEAD, NULL);
	t_weak_ptr   test_ptr = sptr_make_shared(src, NULL);
	t_strong_ptr res;

	assert(sptr_get_wc(test_ptr) == 1);
	assert(sptr_get_rc(test_ptr) == 1);
	res = sptr_strengthen(test_ptr);
	assert(sptr_get_wc(res) == 0);
	assert(sptr_get_rc(res) == 2);
	free(src);
	return;
}

void test_sptr_weaken(void) {
	t_strong_ptr src = sptr_make_shared((void *)0xDEAD, NULL);
	t_weak_ptr   res = sptr_make_shared(src, NULL);

	assert(sptr_get_rc(src) == 1);
	assert(sptr_get_wc(src) == 1);
	assert(sptr_get(src) == (void *)0xDEAD);
	res = sptr_weaken(src);
	assert(res != NULL);
	assert(sptr_get_wc(res) == 2);
	assert(sptr_get_rc(res) == 0);
	free(src);
	return;
}

void test_sptr_steal(void) {
	t_uniq_ptr t1 = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr t2 = sptr_make_uniq(t1, NULL);

	// state
	assert(t1.owner == true && t2.owner == false);
	assert(t1.rcbox == t2.rcbox && t1.rcbox != NULL);
	sptr_steal(&t2, &t1);
	assert(t1.owner == false && t2.owner == true);
	free(t1.rcbox);
	return;
}
