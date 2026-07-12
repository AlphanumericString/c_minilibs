#include <stdio.h>
#include <stdlib.h>

#include "dsu_tests.h"

int main(void) {
	LOG("");
	test_dsu_new();
	test_dsu_new_from_array();
	LOG("");
	test_dsu_init();
	test_dsu_init_from_arrays();
	test_dsu_grow();
	LOG("");
	test_dsu_clr();
	test_dsu_destroy();
	LOG("");
	test_dsu_max_depth();
	test_dsu_size();
	test_dsu_nb_components();
	LOG("");
	test_dsu_is_related();
	test_dsu_strict_is_related();
	LOG("");
	test_dsu_validate();
	test_dsu_union();
	LOG("");
	test_dsu_find();
	test_dsu_strict_find();
	LOG("");
	test_dsu_compress_all();

	printf("All da tests completed successfully.\n");
	return (EXIT_SUCCESS);
}
