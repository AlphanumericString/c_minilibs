#include <assert.h>

#include "tls_tests.h"

void test_vtls_get(void) {
	assert(vtls_get("a") == NULL);
	vtls_set("a", (void *)0xDEAD, sizeof(void *));
	assert(vtls_get("a") != NULL);
	assert(vtls_get("a") == (void *)0xDEAD);
	vtls_rm("a");
}

void test_vtls_len(void) {
	assert(vtls_len() == 0);
	vtls_set("a", (void *)0xDEAD, sizeof(void *));
	assert(vtls_len() == 1);
	vtls_set("b", (void *)0xDEAD, sizeof(void *));
	assert(vtls_len() == 2);
	vtls_rm("a");
	vtls_rm("b");
	return;
}
