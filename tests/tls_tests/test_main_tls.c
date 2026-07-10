#include <pthread.h>
#include <stdlib.h>

#define C_MINI__FTLS_GET
#include "tls_tests.h"

static int ctls_tests(void) {
	test_ctls_dump_typewrap();
	test_ctls_get();
	test_ctls_has();
	test_ctls_len();
	test_ctls_rm();
	test_ctls_set();
	test_ctls_separate();

	printf("All ctls tests completed successfully.\n");
	return (EXIT_SUCCESS);
}

static int ftls_tests(void) {
	test_ftls_get();
	test_ftls_separate();

	printf("All ftls tests completed successfully.\n");
	return (EXIT_SUCCESS);
}

static int vtls_tests(void) {
	test_vtls_rm();
	test_vtls_get();
	test_vtls_len();
	test_vtls_has();
	test_vtls_set();

	printf("All vtls tests completed successfully.\n");
	return (EXIT_SUCCESS);
}

int main(void) { // key setup done at constructor time
	ctls_tests();
	ftls_tests();
	vtls_tests();
	printf("All TLS tests completed successfully.\n");
	return (EXIT_SUCCESS);
}
