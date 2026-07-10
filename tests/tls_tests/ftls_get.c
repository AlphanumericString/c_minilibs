#include <assert.h>

#include "tls_tests.h"

void test_ftls_get() {
	t_ftls *val;

	val          = ftls_get();
	val->value_a = 42;
	val->value_b = -42;
	assert(val);
	assert(ftls_get()->value_a == 42);
	assert(ftls_get()->value_b == -42);
	return;
}
