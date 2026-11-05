#include <assert.h>

#include "tls_tests.h"

NO_COV void test_ctls_get(void) {
	t_cm_ctls_typewrap val;
	void              *data;

	val.value.i = 42;
	val.type    = CTLS_INT;
	assert(ctls_get("a") == NULL);
	ctls_set("a", val);
	assert(ctls_get("a") != NULL);
	assert(ctls_get("a")->type == CTLS_INT);
	assert(ctls_get("a")->value.i == 42);
	ctls_rm("a");
	assert(ctls_len() == 0);

	data = pthread_getspecific(ctls_key);
	assert(data != NULL);                             // was already init-ed
	assert(pthread_setspecific(ctls_key, NULL) == 0); // set to non-init val
	assert(ctls_get("a") == NULL);                    // fails (no ctls)
	assert(pthread_getspecific(ctls_key) == NULL);    // ctls_get hasn't init
	assert(pthread_setspecific(ctls_key, data) == 0); // reset
}

NO_COV void test_ctls_len(void) {
	t_cm_ctls_typewrap val;
	void              *data;

	assert(ctls_len() == 0);
	val.value.i = 42;
	val.type    = CTLS_INT;
	ctls_set("a", val);
	assert(ctls_len() == 1);
	ctls_set("b", val);
	assert(ctls_len() == 2);
	ctls_set("c", val);
	assert(ctls_len() == 3);
	ctls_rm("a");
	assert(ctls_len() == 2);
	ctls_rm("b");
	assert(ctls_len() == 1);
	ctls_rm("c");
	assert(ctls_len() == 0);

	data = pthread_getspecific(ctls_key);
	assert(data != NULL);                             // was already init-ed
	assert(pthread_setspecific(ctls_key, NULL) == 0); // set to non-init val
	assert(ctls_len() == 0);                          // 0 (no ctls)
	assert(pthread_getspecific(ctls_key) == NULL);    // ctls_get hasn't init
	assert(pthread_setspecific(ctls_key, data) == 0); // reset
}
