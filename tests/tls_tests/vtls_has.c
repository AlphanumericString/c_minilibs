#include <assert.h>

#include "tls_tests.h"

void test_vtls_has(void) {
	void *data;

	assert(vtls_has(NULL) == false);
	assert(vtls_has("a") == false);
	assert(vtls_has("b") == false);
	vtls_set("a", (void *)0xDEAD, sizeof(void *));
	assert(vtls_has("a") == true);
	assert(vtls_has("b") == false);
	vtls_set("b", (void *)0xBEEF, sizeof(void *));
	assert(vtls_has("a") == true);
	assert(vtls_has("b") == true);

	data = pthread_getspecific(vtls_key);
	assert(data != NULL);                             // was already init-ed
	assert(pthread_setspecific(vtls_key, NULL) == 0); // set to non-init val
	assert(vtls_has("a") == false);                   // fails (no vtls)
	assert(vtls_has("b") == false);                   // fails (no vtls)
	assert(pthread_getspecific(vtls_key) == NULL);    // func hasn't init vtls
	assert(pthread_setspecific(vtls_key, data) == 0); // reset

	vtls_rm("a");
	vtls_rm("b");
	return;
}
