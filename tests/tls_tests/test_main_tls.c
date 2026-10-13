#include <pthread.h>
#include <stdlib.h>

#define C_MINI__FTLS_GET
#include "tls_tests.h"

static int ctls_tests(void) {
	LOG("");
	LOG("Dump");
	test_ctls_dump_typewrap();
	LOG("");
	LOG("Get");
	test_ctls_get();
	LOG("");
	LOG("Has");
	test_ctls_has();
	LOG("");
	LOG("Len");
	test_ctls_len();
	LOG("");
	LOG("Rm");
	test_ctls_rm();
	LOG("");
	LOG("Set");
	test_ctls_set();
	LOG("");
	LOG("Separate");
	test_ctls_separate();

	printf("All ctls tests completed successfully.\n");
	return (EXIT_SUCCESS);
}

static int ftls_tests(void) {
	LOG("Get");
	test_ftls_get();
	LOG("Separate");
	test_ftls_separate();

	printf("All ftls tests completed successfully.\n");
	return (EXIT_SUCCESS);
}

static int vtls_tests(void) {
	LOG("Rm");
	test_vtls_rm();
	LOG("Get");
	test_vtls_get();
	LOG("Lenght");
	test_vtls_len();
	LOG("Has");
	test_vtls_has();
	LOG("Set");
	test_vtls_set();

	printf("All vtls tests completed successfully.\n");
	return (EXIT_SUCCESS);
}

int __attribute__((no_profile_instrument_function)) main(void) {
	LOG("");
	LOG("");
	LOG("\t\tCTLS");
	LOG("");
	LOG("");
	ctls_tests();
	LOG("");
	LOG("");
	LOG("\t\tFTLS");
	LOG("");
	LOG("");
	ftls_tests();
	LOG("");
	LOG("");
	LOG("\t\tVTLS");
	LOG("");
	LOG("");
	vtls_tests();

	printf("All TLS tests completed successfully.\n");
	return (EXIT_SUCCESS);
}
