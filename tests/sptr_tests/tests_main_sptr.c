#include <assert.h>
#include <unistd.h>

#include "sptr_tests.h"

int main(void) {
	LOG("")
	LOG("Change")
	LOG("")
	test_sptr_strengthen();
	test_sptr_weaken();
	test_sptr_steal();

	LOG("")
	LOG("Copy")
	LOG("")
	test_sptr_copys();
	test_sptr_copyw();
	test_sptr_copyu();

	LOG("")
	LOG("Delete")
	LOG("")
	test_sptr_deletes();
	test_sptr_deletew();
	test_sptr_deleteu();

	LOG("")
	LOG("Dump")
	LOG("")
	test_sptr_dumps();
	test_sptr_dumpw();
	test_sptr_dumpu();

	LOG("")
	LOG("Get")
	LOG("")
	test_sptr_gets();
	test_sptr_getw();
	test_sptr_getu();
	test_sptr_getsc();
	test_sptr_getwc();
	test_sptr_getsc();
	test_sptr_get_rcs();
	test_sptr_get_rcw();
	test_sptr_get_rcu();
	test_sptr_get_wcs();
	test_sptr_get_wcw();
	test_sptr_get_wcu();

	LOG("")
	LOG("Is")
	LOG("")
	test_sptr_is_uniqs();
	test_sptr_is_uniqw();
	test_sptr_is_uniqu();
	test_sptr_is_shareds();
	test_sptr_is_sharedw();
	test_sptr_is_sharedu();
	test_sptr_is_owns();
	test_sptr_is_ownw();
	test_sptr_is_ownu();

	LOG("")
	LOG("Make")
	LOG("")
	test_sptr_make_shared_from_raw();
	test_sptr_make_shared_from_weak();
	test_sptr_make_shared_from_strong();
	test_sptr_make_shared_from_uniq();
	test_sptr_make_uniq_from_raw();
	test_sptr_make_uniq_from_strong();
	test_sptr_make_uniq_from_uniq();

	return (0);
}
