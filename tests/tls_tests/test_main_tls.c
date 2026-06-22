#include <pthread.h>
#include <stdlib.h>

#define C_MINI__FTLS_GET
#include "tls_tests.h"

// // tries to get toto then increment and print.
// static int ctls_tests(void) {
// 	t_ctls_typewrap *a;
// 	pthread_t        handle1;
// 	pthread_t        handle2;
// 	void            *ret;
//
// 	a = ctls_get("toto");
// 	printf("main:");
// 	a ? ctls_dump_typewrap(*a) : printf("no toto");
// 	printf("\n");
//
// 	ctls_set("toto", (t_ctls_typewrap){.type = CTLS_INT, .value.i = 42});
// 	a = ctls_get("toto");
// 	printf("main:");
// 	a ? ctls_dump_typewrap(*a) : printf("no toto");
// 	printf("\n");
//
// 	pthread_create(&handle1, NULL, ctls_dummy, " jcvd");
// 	pthread_create(&handle2, NULL, ctls_dummy, "trump");
//
// 	pthread_join(handle1, &ret);
// 	pthread_join(handle2, &ret);
// 	return EXIT_SUCCESS;
// }

// static int ftls_tests(void) {
// 	pthread_t handle1;
// 	pthread_t handle2;
// 	void     *ret;
// 	t_ftls   *a;
//
// 	a = ftls_get();
// 	printf("main:");
// 	a ? dump_mine_node(*a) : printf("no ftls");
// 	printf("\n");
//
// 	a->value_a       = 42;
// 	a->value_b = 99;
// 	a              = ftls_get();
// 	printf("main:");
// 	a ? dump_mine_node(*a) : printf("no ftls");
// 	printf("\n");
//
// 	pthread_create(&handle1, NULL, ftls_dummy, " jcvd");
// 	pthread_create(&handle2, NULL, ftls_dummy, "trump");
//
// 	pthread_join(handle1, &ret);
// 	pthread_join(handle2, &ret);
// 	printf("All ftls tests completed successfully.\n");
// 	return EXIT_SUCCESS;
// }

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
