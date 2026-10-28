#define C_MINI__TWRAP_IMPL
#include "ill_tests.h"

NO_COV void ill_main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Init", )
	test_ill_init();
	test_ill_copy();
	test_ill_ntoe();
	test_ill_eton();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Is", )
	test_ill_is_isolated();
	test_ill_is_circular();
	test_ill_is_sorted();
	test_ill_has();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Insert", )
	test_ill_push();
	test_ill_push_back();
	test_ill_append();
	test_ill_nappend();
	test_ill_npush();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Remove", )
	test_ill_pop();
	test_ill_pop_back();
	test_ill_remove_after();
	test_ill_remove_node();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Find", )
	test_ill_find();
	test_ill_find_prev();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_ill_get();
	test_ill_get_prev();
	test_ill_get_min();
	test_ill_get_max();
	test_ill_get_kth();
	test_ill_get_tail();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Length", )
	test_ill_length();
	test_ill_nlength();
	test_ill_match_nb();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Apply", )
	test_ill_foreach();
	test_ill_for();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Reverse", )
	test_ill_reverse();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All ILL tests completed successfully.", );
}

NO_COV void idl_main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Init", )
	test_idl_init();
	test_idl_copy();
	test_idl_ntoe();
	test_idl_eton();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Is", )
	test_idl_is_isolated();
	test_idl_is_circular();
	test_idl_is_sorted();
	test_idl_has();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Insert", )
	test_idl_push();
	test_idl_push_back();
	test_idl_append();
	test_idl_nappend();
	test_idl_npush();
	test_idl_prepend();
	test_idl_nprepend();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Remove", )
	test_idl_pop();
	test_idl_pop_back();
	test_idl_remove_after();
	test_idl_remove_node();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Find", )
	test_idl_find();
	test_idl_find_prev();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_idl_get();
	test_idl_get_prev();
	test_idl_get_min();
	test_idl_get_max();
	test_idl_get_kth();
	test_idl_get_tail();
	test_idl_get_head();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Length", )
	test_idl_length();
	test_idl_nlength();
	test_idl_match_nb();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Apply", )
	test_idl_foreach();
	test_idl_for();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Reverse", )
	test_idl_reverse();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All IDL tests completed successfully.", );
}

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", );
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", );
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tILL", );
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", );
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", );
	ill_main();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", );
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", );
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tIDL", );
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", );
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", );
	idl_main();

	CM_LOG(C_MINI__LEVEL_ALWAYS,
	       "All intrusive lists tests completed successfully.", )
	return (0);
}
