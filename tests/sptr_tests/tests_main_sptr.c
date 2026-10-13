#include <assert.h>

#include "sptr_tests.h"

int __attribute__((no_profile_instrument_function)) main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Change");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	test_sptr_strengthen();
	test_sptr_weaken();
	test_sptr_transfer_o();
	test_sptr_move_o();
	test_sptr_yield();
	test_sptr_swap();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Copy");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	test_sptr_copys();
	test_sptr_copyw();
	test_sptr_copyo();
	test_sptr_copyu();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Delete");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	test_sptr_deletes();
	test_sptr_deletew();
	test_sptr_deleteo();
	test_sptr_deleteu();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Dump");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	test_sptr_to_string_s();
	test_sptr_to_string_w();
	test_sptr_to_string_o();
	test_sptr_to_string_u();
	test_sptr_dumps();
	test_sptr_dumpw();
	test_sptr_dumpo();
	test_sptr_dumpu();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	test_sptr_gets();
	test_sptr_getw();
	test_sptr_geto();
	test_sptr_getu();
	test_sptr_getsc();
	test_sptr_getwc();
	test_sptr_getoc();
	test_sptr_getuc();
	// counts
	test_sptr_get_rcs();
	test_sptr_get_rcw();
	test_sptr_get_rco();
	test_sptr_get_rcu();
	test_sptr_get_wcs();
	test_sptr_get_wcw();
	test_sptr_get_wco();
	test_sptr_get_wcu();
	test_sptr_get_dtors();
	test_sptr_get_dtorw();
	test_sptr_get_dtoro();
	test_sptr_get_dtoru();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Set");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	test_sptr_set_dtors();
	test_sptr_set_dtorw();
	test_sptr_set_dtoro();
	test_sptr_set_dtoru();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Is");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	test_sptr_is_uniqs();
	test_sptr_is_uniqw();
	test_sptr_is_uniqo();
	test_sptr_is_uniqu();
	test_sptr_is_shareds();
	test_sptr_is_sharedw();
	test_sptr_is_sharedo();
	test_sptr_is_sharedu();
	test_sptr_is_sole_box_owns();
	test_sptr_is_sole_box_ownw();
	test_sptr_is_sole_box_owno();
	test_sptr_is_sole_box_ownu();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Make");
	CM_LOG(C_MINI__LEVEL_ALWAYS, "");
	test_sptr_make_shared_from_raw();
	test_sptr_make_shared_from_weak();
	test_sptr_make_shared_from_strong();
	test_sptr_make_shared_from_owned();
	test_sptr_make_shared_from_uniq();
	test_sptr_make_weak_from_weak();
	test_sptr_make_weak_from_strong();
	test_sptr_make_owned_from_raw();
	test_sptr_make_owned_from_strong();
	test_sptr_make_owned_from_owned();
	test_sptr_make_owned_from_uniq();
	test_sptr_make_uniq_from_raw();
	test_sptr_make_uniq_from_weak();
	test_sptr_make_uniq_from_strong();
	test_sptr_make_uniq_from_owned();
	test_sptr_make_uniq_from_uniq();

	CM__SPTR_TODO("threaded test where rc and wc change for atomic");

	return (0);
}
