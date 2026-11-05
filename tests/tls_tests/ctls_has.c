#include <assert.h>

#include "tls_tests.h"

NO_COV void test_ctls_has(void) {
	t_cm_ctls_typewrap val;
	void              *data;

	val.value.i = 42;
	val.type    = CTLS_INT;
	assert(ctls_has("a") == false);
	ctls_set("a", val);
	assert(ctls_has("a") == true);
	ctls_rm("a");
	assert(ctls_get("a") == false);
	assert(ctls_get("b") == false);
	ctls_set("a", val);
	ctls_set("b", val);
	assert(ctls_has("a") == true);
	assert(ctls_has("b") == true);
	ctls_rm("b");
	assert(ctls_has("a") == true);
	assert(ctls_has("b") == false);
	ctls_rm("a");
	assert(ctls_get("a") == false);
	assert(ctls_get("b") == false);

	data = pthread_getspecific(ctls_key);
	assert(data != NULL);                             // was already init-ed
	assert(pthread_setspecific(ctls_key, NULL) == 0); // set to non-init val
	assert(ctls_has("a") == false);                   // fails (no ctls)
	assert(pthread_getspecific(ctls_key) == NULL);    // ctls_get hasn't init
	assert(pthread_setspecific(ctls_key, data) == 0); // reset
}
