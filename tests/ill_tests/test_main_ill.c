#include <stdio.h>
#include <stdlib.h>

#include "ill_tests.h"

void ill_main(void) {
	test_ill_init();
	test_ill_ntoe();
	test_ill_is();
	test_ill_insert();
	test_ill_rm();
	test_ill_find();
	test_ill_get();
	test_ill_gettail();
	test_ill_length();
	test_ill_apply();
	test_ill_reverse();
	printf("All ILL tests completed successfully.\n");
}

void idl_main(void) {
	test_idl_init();
	test_idl_ntoe();
	test_idl_is();
	test_idl_insert();
	test_idl_rm();
	test_idl_find();
	test_idl_get();
	test_idl_gettail();
	test_idl_length();
	test_idl_apply();
	test_idl_reverse();
	printf("All IDL tests completed successfully.\n");
}

int main(void) {
	ill_main();
	idl_main();
	printf("All intrusive lists tests completed successfully.\n");
	return (EXIT_SUCCESS);
}
