#include <assert.h>

#include "tls_tests.h"

void test_ctls_rm(void) {
	t_ctls_typewrap val;
	void           *data;

	val.value.i = 42;
	val.type    = CTLS_INT;
	ctls_set("a", val);
	assert(ctls_get("a") != NULL);
	assert(ctls_rm("a") == true);
	assert(ctls_get("a") == NULL);
	ctls_set("a", val);
	ctls_set("b", val);
	assert(ctls_rm("a") == true);
	assert(ctls_rm("a") == false);
	assert(ctls_rm("z") == false);

	assert(ctls_has("b") == true);
	assert(ctls_rm("b") == true);
	assert(ctls_has("b") == false);

	data = pthread_getspecific(ctls_key);
	assert(data != NULL);                             // was already init-ed
	assert(pthread_setspecific(ctls_key, NULL) == 0); // set to non-init val
	assert(ctls_rm("a") == false);                    // fails (no ctls)
	assert(pthread_getspecific(ctls_key) == NULL);    // ctls_get hasn't init
	assert(pthread_setspecific(ctls_key, data) == 0); // reset
}
