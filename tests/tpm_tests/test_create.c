#include <assert.h>
#include <stdlib.h>

#include "tpm_tests.h"

NO_COV void test_tpm_init(void) {
	t_tpm  t;
	t_tpm *ret;

	ret = tpm_init(&t, 2);
	assert(ret == &t);
	assert(tpm_get_nbthread(&t) == 2);
	assert(atomic_load(&t.nb_threads) == 2);
	tpm_delete(&t, NULL);

	// bad arguments: nothing is created
	assert(tpm_init(NULL, 2) == NULL);
	assert(tpm_init(&t, (size_t)C_MINI__TPM_THREADS_MAX_NB + 1) == NULL);
}

NO_COV void test_tpm_new(void) {
	t_tpm *t;

	t = tpm_new(3);
	assert(t != NULL);
	assert(tpm_get_nbthread(t) == 3);
	tpm_free(t, NULL);

	// allocation failure of the pool struct itself
	alloc_reset_counter();
	alloc_set_call_limit(0);
	assert(tpm_new(2) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	// above the hard cap
	assert(tpm_new((size_t)C_MINI__TPM_THREADS_MAX_NB + 1) == NULL);
}

NO_COV void test_tpm_new_default(void) {
	t_tpm *t;

	t = tpm_new(0);
	assert(t != NULL);
	assert(tpm_get_nbthread(t) == C_MINI__TPM_THREADS_DFLT_NB);
	tpm_free(t, NULL);
}
