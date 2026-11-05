#include <assert.h>

#include "tls_tests.h"

NO_COV void test_cm_ftls_get(void) {
	t_cm_ftls *val;
	void      *prev;

	val          = ftls_get();
	val->value_a = 42;
	val->value_b = -42;
	assert(val);
	assert(ftls_get()->value_a == 42);
	assert(ftls_get()->value_b == -42);
	prev = pthread_getspecific(ftls_key);

	pthread_setspecific(ftls_key, NULL);
	assert(ftls_get() != NULL);
	assert(pthread_getspecific(ftls_key) != NULL);
	free(ftls_get());
	pthread_setspecific(ftls_key, NULL);

	alloc_set_call_limit(0);
	assert(ftls_get() == NULL);
	assert(pthread_getspecific(ftls_key) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	pthread_setspecific(ftls_key, prev);
	return;
}
