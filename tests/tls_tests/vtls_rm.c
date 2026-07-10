#include <assert.h>

#include "tls_tests.h"

// remove
void test_vtls_rm(void) {
	void *a = (void *)0xDEAD;

	vtls_set("a", a, sizeof(a));
	vtls_get("a");
	assert(vtls_has("a") == true);
	assert(vtls_rm("a") == true);
	assert(vtls_rm("a") == false);
	assert(vtls_has("a") == false);
	return;
}
