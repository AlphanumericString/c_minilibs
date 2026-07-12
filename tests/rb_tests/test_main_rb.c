#include <stdio.h>
#include <stdlib.h>

#include "rb_tests.h"

int main(void) {
	LOG("");
	LOG("Create");
	test_rb_create();
	test_rb_init();
	test_rb_from_array();
	test_rb_new();
	LOG("");
	LOG("Delete");
	test_rb_destroy();
	test_rb_clr();
	LOG("");
	LOG("Querry");
	test_rb_full();
	test_rb_empty();
	test_rb_size();
	test_rb_cappacity();
	test_rb_remaining_space();
	LOG("");
	LOG("Rm");
	test_rb_drop();
	test_rb_pop();
	test_rb_npop();
	LOG("");
	LOG("Peek");
	test_rb_peek();
	test_rb_npeek();
	LOG("");
	LOG("Push");
	test_rb_push();
	test_rb_npush();
	test_rb_push_safe();
	test_rb_npush_safe();
	LOG("");
	LOG("Foreach");
	test_rb_apply();
	test_rb_naplly();
	LOG("");
	LOG("Filter");
	test_rb_filter();
	LOG("");
	LOG("Access");
	test_rb_at();
	test_rb_nat();
	test_rb_get();
	test_rb_nget();
	LOG("");
	LOG("Find");
	test_rb_find();
	LOG("");
	LOG("Map");
	test_rb_map();
	test_rb_nmap();
	LOG("");
	LOG("Grow");
	test_rb_grow();

	printf("All rb tests completed successfully.\n");
	return (EXIT_SUCCESS);
}
