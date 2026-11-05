#define C_MINI__TWRAP_IMPL
#include "ill_tests.h"

NO_COV void ill_main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Init", )
	test_cm_ill_init();
	test_cm_ill_copy();
	test_cm_ill_ntoe();
	test_cm_ill_eton();
	test_cm_ill_advance_it();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Is", )
	test_cm_ill_is_isolated();
	test_cm_ill_is_circular();
	test_cm_ill_is_sorted();
	test_cm_ill_has();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Insert", )
	test_cm_ill_push();
	test_cm_ill_push_back();
	test_cm_ill_append();
	test_cm_ill_nappend();
	test_cm_ill_npush();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Remove", )
	test_cm_ill_pop();
	test_cm_ill_pop_back();
	test_cm_ill_remove_after();
	test_cm_ill_remove_node();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Find", )
	test_cm_ill_find();
	test_cm_ill_find_prev();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_cm_ill_get();
	test_cm_ill_get_prev();
	test_cm_ill_get_min();
	test_cm_ill_get_max();
	test_cm_ill_get_kth();
	test_cm_ill_get_tail();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Length", )
	test_cm_ill_length();
	test_cm_ill_nlength();
	test_cm_ill_match_nb();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Apply", )
	test_cm_ill_foreach();
	test_cm_ill_for();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Reverse", )
	test_cm_ill_reverse();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Sort", )
	test_cm_ill_sort();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Filter", )
	test_cm_ill_filter();
	test_cm_ill_filter_out();
	test_cm_ill_rm_if();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Unique", )
	test_cm_ill_unique();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All ILL tests completed successfully.", );
}

NO_COV void idl_main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Init", )
	test_cm_idl_init();
	test_cm_idl_copy();
	test_cm_idl_ntoe();
	test_cm_idl_eton();
	test_cm_idl_advance_it();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Is", )
	test_cm_idl_is_isolated();
	test_cm_idl_is_circular();
	test_cm_idl_is_sorted();
	test_cm_idl_has();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Insert", )
	test_cm_idl_push();
	test_cm_idl_push_back();
	test_cm_idl_append();
	test_cm_idl_nappend();
	test_cm_idl_npush();
	test_cm_idl_prepend();
	test_cm_idl_nprepend();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Remove", )
	test_cm_idl_pop();
	test_cm_idl_pop_back();
	test_cm_idl_remove_after();
	test_cm_idl_remove_node();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Find", )
	test_cm_idl_find();
	test_cm_idl_find_prev();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_cm_idl_get();
	test_cm_idl_get_prev();
	test_cm_idl_get_min();
	test_cm_idl_get_max();
	test_cm_idl_get_kth();
	test_cm_idl_get_tail();
	test_cm_idl_get_head();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Length", )
	test_cm_idl_length();
	test_cm_idl_nlength();
	test_cm_idl_match_nb();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Apply", )
	test_cm_idl_foreach();
	test_cm_idl_for();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Reverse", )
	test_cm_idl_reverse();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Sort", )
	test_cm_idl_sort();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Filter", )
	test_cm_idl_filter();
	test_cm_idl_filter_out();
	test_cm_idl_rm_if();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Unique", )
	test_cm_idl_unique();

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
