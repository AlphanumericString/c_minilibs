#include <assert.h>
#include <stddef.h>

#include "tls_tests.h"

void test_ctls_set(void) {
	t_ctls_typewrap val;
	const char     *names[C_MINI__CTLS_CAP + 1] = {
        "a", "b", "c", "d", "e", "f", "g", "h", "i",
        "j", "k", "l", "m", "n", "o", "p", "q",
    };
	size_t i;

	val.value.i = 42;
	val.type    = CTLS_INT;
	for (i = 0; i < C_MINI__CTLS_CAP; i++)
		assert(ctls_set(names[i], val) == true);
	assert(ctls_set(names[i], val) != true);
	assert(ctls_len() == C_MINI__CTLS_CAP);
	val.value.l = '+';
	val.type    = CTLS_LONG;
	ctls_set(names[3], val);
	assert(ctls_get(names[2])->type == CTLS_INT);
	assert(ctls_get(names[3])->type == CTLS_LONG);
	assert(ctls_get(names[3])->value.l != ctls_get(names[2])->value.i);
	assert(ctls_get(names[3])->value.l == '+');
	for (i = 0; i < C_MINI__CTLS_CAP; i++)
		ctls_rm(names[i]);
	assert(ctls_len() == 0);
}
