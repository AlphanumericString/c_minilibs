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

NO_COV void test_cm_tpm_new_init_fail(void) {
	// tpm_init's pthread_cond_init failure makes tpm_new free the pool and
	// report the failure.
	pthread_cond_init_set_call_limit(0);
	assert(tpm_new(2) == NULL);
	pthread_cond_init_set_call_limit(PTHREAD_DFT_LIMIT);
}

NO_COV void test_cm_tpm_init_cond_fail(void) {
	t_cm_tpm t;

	// init_q fails: nb_threads is flagged -1 and no worker is spawned.
	pthread_cond_init_set_call_limit(0);
	assert(tpm_init(&t, 2) == NULL);
	assert(atomic_load(&t.nb_threads) == (size_t)-1);
	pthread_cond_init_set_call_limit(PTHREAD_DFT_LIMIT);
}

NO_COV void test_cm_tpm_init_thread_fail(void) {
	t_cm_tpm t;

	// the first worker spawns, the second fails: the pool is stopped, the
	// already-running worker is cancelled and nb_threads is flagged -1.
	pthread_create_set_call_limit(1);
	assert(tpm_init(&t, 2) == NULL);
	assert(atomic_load(&t.nb_threads) == (size_t)-1);
	pthread_join(t.threads[0].id_absolute, NULL);
	pthread_create_set_call_limit(PTHREAD_DFT_LIMIT);
	pthread_cond_destroy(&t.q_cond);
	pthread_mutex_destroy(&t.q_lock);
}
