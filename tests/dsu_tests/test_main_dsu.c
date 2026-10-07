#include <stdio.h>
#include <stdlib.h>

#include "cdsu_tests.h"
#include "rdsu_tests.h"

int cdsu_main_tests() {
	LOG("");
	LOG("Create");
	test_cdsu_new();
	test_cdsu_new_from_array();
	test_cdsu_init();
	test_cdsu_init_from_arrays();
	LOG("");
	LOG("Delete");
	test_cdsu_clr();
	test_cdsu_destroy();
	test_cdsu_free();
	LOG("");
	LOG("Infos");
	test_cdsu_max_depth();
	test_cdsu_size();
	test_cdsu_nb_components();
	LOG("");
	LOG("Grow");
	test_cdsu_grow();
	LOG("");
	LOG("Links (union + compress)");
	test_cdsu_strict_find();
	test_cdsu_find();
	test_cdsu_compress_all();
	test_cdsu_union();
	LOG("");
	LOG("Querry");
	test_cdsu_validate();
	test_cdsu_is_related();
	test_cdsu_strict_is_related();

	printf("All cdsu tests completed successfully.\n");
	return (EXIT_SUCCESS);
}
int rdsu_main_tests() {
	LOG("");
	LOG("Create");
	test_rdsu_new();
	test_rdsu_new_from_array();
	test_rdsu_init();
	test_rdsu_init_from_arrays();
	LOG("");
	LOG("Delete");
	test_rdsu_clr();
	test_rdsu_destroy();
	test_rdsu_free();
	LOG("");
	LOG("Infos");
	test_rdsu_max_depth();
	test_rdsu_size();
	test_rdsu_nb_components();
	LOG("");
	LOG("Grow");
	test_rdsu_grow();
	LOG("");
	LOG("Links (union + compress)");
	test_rdsu_find();
	test_rdsu_compress_all();
	test_rdsu_union();
	LOG("");
	LOG("Querry");
	test_rdsu_validate();
	test_rdsu_is_related();
	LOG("");
	LOG("Rewind");
	test_rdsu_save_state();
	test_rdsu_rewind();
	test_rdsu_rewind_to_state();
	test_rdsu_rewind_clear();
	test_rdsui_push_rewind();
	printf("All rdsu tests completed successfully.\n");
	return (EXIT_SUCCESS);
}

int main(void) {
	LOG("");
	LOG("");
	LOG("\t\tCDSU");
	LOG("");
	LOG("");
	cdsu_main_tests();
	LOG("");
	LOG("");
	LOG("\t\tRDSU");
	LOG("");
	LOG("");
	rdsu_main_tests();

	printf("All dsu tests completed successfully.\n");
	return (EXIT_SUCCESS);
}
