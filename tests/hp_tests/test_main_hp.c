#include <stdio.h>

#include "hp_tests.h"

int main(void) {
	LOG("");
	LOG("Create");
	test_hp_create();
	test_hp_init();
	LOG("");
	LOG("Delete");
	test_hp_clear();
	test_hp_delete();
	test_hp_free();
	LOG("");
	LOG("Push");
	test_hp_min_push();
	test_hp_max_push();
	LOG("");
	LOG("Pop");
	test_hp_min_pop();
	test_hp_max_pop();
	LOG("");
	LOG("Rm");
	test_hp_max_rm();
	test_hp_min_rm();
	LOG("");
	LOG("Foreach");
	test_hp_foreach();
	test_hp_foreach_in();
	test_hp_foreach_pre();
	test_hp_foreach_post();

	printf("All hp tests completed successfully.\n");
}
