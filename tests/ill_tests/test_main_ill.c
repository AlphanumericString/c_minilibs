#define C_MINI__TWRAP_IMPL
#include "ill_tests.h"

NO_COV static void ill_elem_main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " elem Init", )
	test_cm_ill_init();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " elem Is", )
	test_cm_ill_is_isolated();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " elem Insert", )
	test_cm_ill_append();
	test_cm_ill_nappend();
	test_cm_ill_push();
	test_cm_ill_npush();
	test_cm_ill_push_back();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " elem Remove", )
	test_cm_ill_pop();
	test_cm_ill_pop_back();
	test_cm_ill_remove_after();
	test_cm_ill_remove();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " elem Find", )
	test_cm_ill_find();
	test_cm_ill_find_prev();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " elem Get", )
	test_cm_ill_get();
	test_cm_ill_get_prev();
	test_cm_ill_get_min();
	test_cm_ill_get_max();
	test_cm_ill_get_kth();
	test_cm_ill_get_tail();
}

NO_COV static void ill_node_main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Init", )
	test_cm_ill_init_node();
	test_cm_ill_copy();
	test_cm_ill_ntoe();
	test_cm_ill_eton();
	test_cm_ill_advance_it();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Is", )
	test_cm_ill_is_isolated_node();
	test_cm_ill_is_circular();
	test_cm_ill_is_sorted();
	test_cm_ill_has();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Insert", )
	test_cm_ill_push_node();
	test_cm_ill_push_back_node();
	test_cm_ill_append_node();
	test_cm_ill_nappend_node();
	test_cm_ill_npush_node();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Remove", )
	test_cm_ill_pop_node();
	test_cm_ill_pop_back_node();
	test_cm_ill_remove_after_node();
	test_cm_ill_remove_node();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Find", )
	test_cm_ill_find_node();
	test_cm_ill_find_prev_node();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Get", )
	test_cm_ill_get_node();
	test_cm_ill_get_prev_node();
	test_cm_ill_get_min_node();
	test_cm_ill_get_max_node();
	test_cm_ill_get_kth_node();
	test_cm_ill_get_tail_node();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Length", )
	test_cm_ill_length();
	test_cm_ill_nlength();
	test_cm_ill_match_nb();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Apply", )
	test_cm_ill_iter();
	test_cm_ill_apply();
	test_cm_ill_napply();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Reverse", )
	test_cm_ill_reverse();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Sort", )
	test_cm_ill_sort();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Filter", )
	test_cm_ill_filter();
	test_cm_ill_filter_out();
	test_cm_ill_rm_if();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Unique", )
	test_cm_ill_unique();
}

NO_COV void ill_main(void) {
	ill_elem_main();
	CM_LOG(C_MINI__LEVEL_ALWAYS,
	       "All ILL element tests completed successfully.", );
	ill_node_main();
	CM_LOG(C_MINI__LEVEL_ALWAYS,
	       "All ILL node tests completed successfully.", );
}

NO_COV static void idl_elem_main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " elem Init", )
	test_cm_idl_init();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " elem Is", )
	test_cm_idl_is_isolated();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " elem Insert", )
	test_cm_idl_append();
	test_cm_idl_nappend();
	test_cm_idl_prepend();
	test_cm_idl_nprepend();
	test_cm_idl_push();
	test_cm_idl_npush();
	test_cm_idl_push_back();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " elem Remove", )
	test_cm_idl_pop();
	test_cm_idl_pop_back();
	test_cm_idl_remove_after();
	test_cm_idl_remove();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " elem Find", )
	test_cm_idl_find();
	test_cm_idl_find_prev();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " elem Get", )
	test_cm_idl_get();
	test_cm_idl_get_prev();
	test_cm_idl_get_min();
	test_cm_idl_get_max();
	test_cm_idl_get_kth();
	test_cm_idl_get_tail();
	test_cm_idl_get_head();
}

NO_COV static void idl_node_main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Init", )
	test_cm_idl_init_node();
	test_cm_idl_copy();
	test_cm_idl_ntoe();
	test_cm_idl_eton();
	test_cm_idl_advance_it();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Is", )
	test_cm_idl_is_isolated_node();
	test_cm_idl_is_circular();
	test_cm_idl_is_sorted();
	test_cm_idl_has();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Insert", )
	test_cm_idl_push_node();
	test_cm_idl_push_back_node();
	test_cm_idl_append_node();
	test_cm_idl_nappend_node();
	test_cm_idl_npush_node();
	test_cm_idl_prepend_node();
	test_cm_idl_nprepend_node();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Remove", )
	test_cm_idl_pop_node();
	test_cm_idl_pop_back_node();
	test_cm_idl_remove_after_node();
	test_cm_idl_remove_node();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Find", )
	test_cm_idl_find_node();
	test_cm_idl_find_prev_node();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Get", )
	test_cm_idl_get_node();
	test_cm_idl_get_prev_node();
	test_cm_idl_get_min_node();
	test_cm_idl_get_max_node();
	test_cm_idl_get_kth_node();
	test_cm_idl_get_tail_node();
	test_cm_idl_get_head_node();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Length", )
	test_cm_idl_length();
	test_cm_idl_nlength();
	test_cm_idl_match_nb();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Apply", )
	test_cm_idl_iter();
	test_cm_idl_apply();
	test_cm_idl_napply();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Reverse", )
	test_cm_idl_reverse();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Sort", )
	test_cm_idl_sort();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Filter", )
	test_cm_idl_filter();
	test_cm_idl_filter_out();
	test_cm_idl_rm_if();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, " node Unique", )
	test_cm_idl_unique();
}

NO_COV void idl_main(void) {
	idl_elem_main();
	CM_LOG(C_MINI__LEVEL_ALWAYS,
	       "All IDL element tests completed successfully.", );
	idl_node_main();
	CM_LOG(C_MINI__LEVEL_ALWAYS,
	       "All IDL node tests completed successfully.", );
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
