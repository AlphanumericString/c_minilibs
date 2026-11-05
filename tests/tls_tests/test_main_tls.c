#include <pthread.h>
#include <stdlib.h>

#define C_MINI__TWRAP_IMPL
#define C_MINI__FTLS_GET
#include "tls_tests.h"

static int ctls_tests(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Dump", )
	test_ctls_dump_typewrap();
	test_ctls_dump_all();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_ctls_get();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Has", )
	test_ctls_has();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Len", )
	test_ctls_len();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Rm", )
	test_ctls_rm();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Set", )
	test_ctls_set();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Separate", )
	test_ctls_separate();

	printf("All ctls tests completed successfully.\n");
	return (EXIT_SUCCESS);
}

static int ftls_tests(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_cm_ftls_get();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Separate", )
	test_cm_ftls_separate();

	printf("All ftls tests completed successfully.\n");
	return (EXIT_SUCCESS);
}

static int vtls_tests(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Dump", )
	test_cm_vtls_dump();
	test_cm_vtls_dump_all();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Rm", )
	test_cm_vtls_rm();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Get", )
	test_cm_vtls_get();
	test_cm_vtls_sizeof();
	test_cm_vtls_getout();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Lenght", )
	test_cm_vtls_len();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Has", )
	test_cm_vtls_has();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Set", )
	test_cm_vtls_set();

	printf("All vtls tests completed successfully.\n");
	return (EXIT_SUCCESS);
}

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tCTLS", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	ctls_tests();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tFTLS", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	ftls_tests();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "\t\tVTLS", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	vtls_tests();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All TLS tests completed successfully.", )
	return (EXIT_SUCCESS);
}
