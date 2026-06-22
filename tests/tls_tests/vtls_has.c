#include <assert.h>
#include <tls/voided_thread_local_storage.h>
#include "tls_tests.h"

// querry
void test_vtls_has(void) {
	assert(vtls_has("a") == false);
	assert(vtls_has("b") == false);
	vtls_set("a", (void*)0xDEAD, sizeof(void*));
	assert(vtls_has("a") == true);
	assert(vtls_has("b") == false);
	vtls_set("b", (void*)0xBEEF, sizeof(void*));
	assert(vtls_has("a") == true);
	assert(vtls_has("b") == true);
	vtls_rm("a");
	vtls_rm("b");
	return;
}
