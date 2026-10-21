#define C_MINI__TWRAP_IMPL
#include "ill_tests.h"

void ill_main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_ill_init();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_ill_ntoe();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_ill_is();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_ill_insert();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_ill_rm();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_ill_find();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_ill_get();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_ill_gettail();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_ill_length();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_ill_foreach();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_ill_reverse();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All ILL tests completed successfully.", )
}

void idl_main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_idl_init();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_idl_ntoe();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_idl_is();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_idl_insert();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_idl_rm();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_idl_find();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_idl_get();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_idl_gettail();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_idl_gethead();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_idl_length();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_idl_foreach();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	test_idl_reverse();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All IDL tests completed successfully.", )
}

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tILL", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	ill_main();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tIDL", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	idl_main();

	CM_LOG(C_MINI__LEVEL_ALWAYS,
	       "All intrusive lists tests completed successfully.", )
	return (0);
}
