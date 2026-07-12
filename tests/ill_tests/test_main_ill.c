#include <stdio.h>
#include <stdlib.h>

#include "ill_tests.h"

void ill_main(void) {
	LOG("");
	test_ill_init();
	LOG("");
	test_ill_ntoe();
	LOG("");
	test_ill_is();
	LOG("");
	test_ill_insert();
	LOG("");
	test_ill_rm();
	LOG("");
	test_ill_find();
	LOG("");
	test_ill_get();
	LOG("");
	test_ill_gettail();
	LOG("");
	test_ill_length();
	LOG("");
	test_ill_apply();
	LOG("");
	test_ill_reverse();

	printf("All ILL tests completed successfully.\n");
}

void idl_main(void) {
	LOG("");
	test_idl_init();
	LOG("");
	test_idl_ntoe();
	LOG("");
	test_idl_is();
	LOG("");
	test_idl_insert();
	LOG("");
	test_idl_rm();
	LOG("");
	test_idl_find();
	LOG("");
	test_idl_get();
	LOG("");
	test_idl_gettail();
	LOG("");
	test_idl_length();
	LOG("");
	test_idl_apply();
	LOG("");
	test_idl_reverse();

	printf("All IDL tests completed successfully.\n");
}

int main(void) {
	LOG("");
	LOG("");
	LOG("\t\tILL");
	LOG("");
	LOG("");
	ill_main();
	LOG("");
	LOG("");
	LOG("\t\tIDL");
	LOG("");
	LOG("");
	idl_main();

	printf("All intrusive lists tests completed successfully.\n");
	return (EXIT_SUCCESS);
}
