#include <assert.h>

#include "tls_tests.h"

void test_ctls_get(void) {
	t_ctls_typewrap val;

	val.value.i = 42;
	val.type    = CTLS_INT;

	ctls_set("a", val);
	assert(ctls_get("a") != NULL);
	assert(ctls_get("a")->type == CTLS_INT);
	assert(ctls_get("a")->value.i == 42);
	ctls_rm("a");
	assert(ctls_len() == 0);
}

void test_ctls_len(void) {
	t_ctls_typewrap val;

	val.value.i = 42;
	val.type    = CTLS_INT;

	assert(ctls_len() == 0);
	ctls_set("a", val);
	assert(ctls_len() != 0);
	assert(ctls_len() == 1);
	ctls_set("b", val);
	ctls_set("c", val);
	assert(ctls_len() != 0);
	assert(ctls_len() == 3);
	ctls_rm("a");
	ctls_rm("b");
	ctls_rm("c");
	assert(ctls_len() == 0);
}
