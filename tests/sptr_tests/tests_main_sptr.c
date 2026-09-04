#include <assert.h>
#include <unistd.h>

#include "sptr_tests.h"

// TODO:
// argument to be made to split uniq and shared into differing types
// pros, cons,
// 1.PROS
//	- typecheck conserved
//	- disambiguation inside functions, less branching
//	- split box for re-use
// 2.CONS
//	- need to type twice as much

int main(void) {
	LOG("")
	LOG("Create")
	LOG("")
	test_sptr_make_sharedv();
	test_sptr_make_sharedsptr();
	test_sptr_make_uniqv();
	test_sptr_make_uniqsptr();

	LOG("")
	LOG("Delete")
	LOG("")
	test_sptr_delete_shared();
	test_sptr_delete_uniq();

	LOG("")
	LOG("Change")
	LOG("")
	test_sptr_strengthen();
	test_sptr_weaken();
	test_sptr_steal();

	LOG("")
	LOG("Copy")
	LOG("")
	test_sptr_copy();
	test_sptr_weak_copy();
	test_sptr_strong_copy();

	LOG("")
	LOG("Get")
	LOG("")
	test_sptr_getdata();
	test_sptr_get_wc();
	test_sptr_get_sc();
	test_sptr_get_free_er();

	return (0);
}
