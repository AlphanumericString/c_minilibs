#include <stdio.h>
#include <stdlib.h>

#include "dsu_tests.h"

int main(void) {
	LOG("");
	LOG("Create");
	test_dsu_new();
	test_dsu_new_from_array();
	test_dsu_init();
	test_dsu_init_from_arrays();
	LOG("");
	LOG("Delete");
	test_dsu_clr();
	test_dsu_destroy();
	test_dsu_free();
	LOG("");
	LOG("Infos");
	test_dsu_max_depth();
	test_dsu_size();
	test_dsu_nb_components();
	LOG("");
	LOG("Grow");
	test_dsu_grow();
	LOG("");
	LOG("Links (union + compress)");
	test_dsu_strict_find();
	test_dsu_find();
	test_dsu_compress_all();
	test_dsu_union();
	LOG("");
	LOG("Querry");
	test_dsu_validate();
	test_dsu_is_related();
	test_dsu_strict_is_related();

	printf("All dsu tests completed successfully.\n");
	return (EXIT_SUCCESS);
}
