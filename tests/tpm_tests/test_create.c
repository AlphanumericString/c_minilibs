#include <assert.h>
#include <stdlib.h>

#include "tpm_tests.h"

NO_COV void test_cm_tpm_init(void) {
	t_cm_tpm  t;
	t_cm_tpm *ret;

	ret = tpm_init(&t, 2);
	assert(ret == &t);
	assert(tpm_get_nbthread(&t) == 2);
	assert(atomic_load(&t.nb_threads) == 2);
	tpm_delete(&t, NULL);

	// bad arguments: nothing is created
	assert(tpm_init(NULL, 2) == NULL);
	assert(tpm_init(&t, (size_t)C_MINI__TPM_THREADS_MAX_NB + 1) == NULL);
}

NO_COV void test_cm_tpm_new(void) {
	t_cm_tpm *t;

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

NO_COV void test_cm_tpm_new_default(void) {
	t_cm_tpm *t;

	t = tpm_new(0);
	assert(t != NULL);
	assert(tpm_get_nbthread(t) == C_MINI__TPM_THREADS_DFLT_NB);
	tpm_free(t, NULL);
}

NO_COV void test_cm_tpm_init_default(void) {
	t_cm_tpm t;

	// a zero thread count falls back to the default, same as tpm_new
	assert(tpm_init(&t, 0) == &t);
	assert(tpm_get_nbthread(&t) == C_MINI__TPM_THREADS_DFLT_NB);
	tpm_delete(&t, NULL);
}

// NOTE(coverage): tpm_create's remaining uncovered regions are the failure
// paths of the pthread primitives, which no test can force today:
//   - tpm_new: tpm_init() returning NULL (line 30)
//   - init_q: pthread_cond_init() failing (lines 48-50)
//   - tpm_init: init_q() failing (line 82)
//   - tpm_init: pthread_create() failing mid-spawn (lines 91-99)
// Covering them needs a __wrap_pthread_cond_init / __wrap_pthread_create pair
// (same shape as the malloc/clock wrappers in common_base/tests.h) driven by a
// call-limit counter. The tests themselves are then trivial: fail on the Nth
// call and assert the pool is torn down and nb_threads is set to -1.
