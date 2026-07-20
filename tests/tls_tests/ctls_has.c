#include <assert.h>

#include "tls_tests.h"

void test_ctls_has(void) {
	t_ctls_typewrap val;

	val.value.i = 42;
	val.type    = CTLS_INT;
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
	assert(ctls_len() == 0);
}
