#include <assert.h>

#include "tls_tests.h"

void test_ctls_rm(void) {
	t_ctls_typewrap val;

	val.value.i = 42;
	val.type    = CTLS_INT;
	ctls_set("a", val);
	assert(ctls_get("a") != NULL);
	ctls_rm("a");
	assert(ctls_get("a") == NULL);
	ctls_set("a", val);
	ctls_set("b", val);
	ctls_rm("a");
	assert(ctls_get("a") == NULL);
	assert(ctls_get("b") != NULL);
	ctls_rm("b");
}
