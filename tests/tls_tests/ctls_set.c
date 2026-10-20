#include <assert.h>
#include <stddef.h>

#include "tls_tests.h"

NO_COV void test_ctls_set(void) {
	t_ctls_typewrap val;
	const char     *names[C_MINI__CTLS_CAP + 1] = {
        "a", "b", "c", "d", "e", "f", "g", "h", "i",
        "j", "k", "l", "m", "n", "o", "p", "q",
    };
	size_t i;
	void  *prev;

	val.value.i = 42;
	val.type    = CTLS_INT;
	for (i = 0; i < C_MINI__CTLS_CAP; i++)
		assert(ctls_set(names[i], val) == true);
	assert(ctls_set(names[i], val) != true);
	assert(ctls_len() == C_MINI__CTLS_CAP);
	val.value.l = '+';
	val.type    = CTLS_LONG;
	assert(3 < C_MINI__CTLS_CAP);
	assert(ctls_set(names[3], val) == true);
	assert(ctls_get(names[2])->type == CTLS_INT);
	assert(ctls_get(names[3])->type == CTLS_LONG);
	assert(ctls_get(names[3])->value.l != ctls_get(names[2])->value.i);
	assert(ctls_get(names[3])->value.l == '+');
	for (i = 0; i < C_MINI__CTLS_CAP; i++)
		ctls_rm(names[i]);
	assert(ctls_len() == 0);

	prev = pthread_getspecific(ctls_key);
	pthread_setspecific(ctls_key, NULL);
	assert(ctls_set(names[0], val) == true);        // set ok
	assert(pthread_getspecific(ctls_key) != prev && // init tls ok
	       pthread_getspecific(ctls_key) != NULL);
	assert(ctls_get(names[0])->value.l == val.value.l); // val ok
	assert(ctls_get(names[0])->type == val.type);
	ctls_rm(names[0]); // clear mem
	free(pthread_getspecific(ctls_key));
	pthread_setspecific(ctls_key, NULL); // reset tls

	alloc_set_call_limit(0);
	assert(ctls_set(names[0], val) == false);      // alloc fails for new tls
	assert(pthread_getspecific(ctls_key) == NULL); // tls still null
	alloc_set_call_limit(ALLOC_DFT_LIMIT);         // reset alloc limit

	pthread_setspecific(ctls_key, prev);
}
