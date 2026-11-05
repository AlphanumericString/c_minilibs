#include <assert.h>

#include "tls_tests.h"

NO_COV void test_cm_vtls_rm(void) {
	void *data;
	void *a = (void *)0xDEAD;

	vtls_set("a", a, sizeof(a));
	assert(vtls_has("a") == true);
	assert(vtls_rm("a") == true);
	assert(vtls_has("a") == false);

	assert(vtls_rm("a") == false);
	assert(vtls_rm("zzz") == false);
	assert(vtls_rm(NULL) == false);

	vtls_set("a", a, sizeof(a));
	assert(vtls_rm("zzz") == false);
	assert(vtls_rm(NULL) == false);

	data = pthread_getspecific(vtls_key);
	assert(data != NULL);                             // was already init-ed
	assert(pthread_setspecific(vtls_key, NULL) == 0); // set to non-init val
	assert(vtls_rm("a") == false);                    // fails (no vtls)
	assert(pthread_getspecific(vtls_key) == NULL);    // func hasn't init vtls
	assert(pthread_setspecific(vtls_key, data) == 0); // reset

	assert(vtls_rm("a") == true);

	return;
}
