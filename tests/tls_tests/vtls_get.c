#include <assert.h>

#include "tls_tests.h"

void test_vtls_get(void) {
	void *data;
	assert(vtls_get(NULL) == NULL);
	assert(vtls_get("a") == NULL);
	vtls_set("a", (void *)0xDEAD, sizeof(void *));
	assert(vtls_get("a") != NULL);
	assert(vtls_get("a") == (void *)0xDEAD);

	data = pthread_getspecific(vtls_key);
	assert(data != NULL);                             // was already init-ed
	assert(pthread_setspecific(vtls_key, NULL) == 0); // set to non-init val
	assert(vtls_get("a") == NULL);                    // fails (no vtls)
	assert(pthread_getspecific(vtls_key) == NULL);    // func hasn't init vtls
	assert(pthread_setspecific(vtls_key, data) == 0); // reset

	vtls_rm("a");
}

void test_vtls_len(void) {
	void *data;

	assert(vtls_len() == 0);
	vtls_set("a", (void *)0xDEAD, sizeof(void *));
	assert(vtls_len() == 1);
	vtls_set("b", (void *)0xDEAD, sizeof(void *));
	assert(vtls_len() == 2);

	data = pthread_getspecific(vtls_key);
	assert(data != NULL);                             // was already init-ed
	assert(pthread_setspecific(vtls_key, NULL) == 0); // set to non-init val
	assert(vtls_len() == 0);                          // 0 (no vtls)
	assert(pthread_getspecific(vtls_key) == NULL);    // func hasn't init vtls
	assert(pthread_setspecific(vtls_key, data) == 0); // reset

	vtls_rm("a");
	vtls_rm("b");
	return;
}
