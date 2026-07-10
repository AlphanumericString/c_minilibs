#include <stdio.h>
#include <stdlib.h>

#include "rb_tests.h"

int main(void) {
	test_rb_create();
	test_rb_init();
	test_rb_from_array();
	test_rb_new();
	test_rb_destroy();
	test_rb_clr();
	test_rb_full();
	test_rb_empty();
	test_rb_size();
	test_rb_cappacity();
	test_rb_free();
	test_rb_drop();
	test_rb_pop();
	test_rb_npop();
	test_rb_peek();
	test_rb_npeek();
	test_rb_push();
	test_rb_npush();
	test_rb_push_safe();
	test_rb_npush_safe();
	test_rb_apply();
	test_rb_naplly();
	test_rb_filter();
	test_rb_at();
	test_rb_nat();
	test_rb_get();
	test_rb_nget();
	test_rb_find();
	test_rb_map();
	test_rb_nmap();
	test_rb_grow();

	printf("All rb tests completed successfully.\n");
	return (EXIT_SUCCESS);
}
