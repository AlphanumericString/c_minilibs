#include <assert.h>
#include <stdint.h>

#include "tpm_tests.h"

NO_COV void test_cm_tpm_add_job(void) {
	t_cm_tpm *t = tpm_new(2);

	cb_reset_counter();
	assert(tpm_add_job(t, cb_work_incr, NULL) != NULL);
	assert(tpm_add_job(t, cb_work_incr, (void *)(uintptr_t)0x1) != NULL);
	assert(tpm_wait_end(t) == true);
	assert(cb_get_counter() == 2);

	// the queue is closed once tpm_wait_end returns: submission is refused
	// and the freshly allocated container is released.
	assert(tpm_add_job(t, cb_work_incr, NULL) == NULL);

	tpm_free(t, NULL);
}

NO_COV void test_cm_tpm_add_job_errors(void) {
	t_cm_tpm *t = tpm_new(2);

	assert(tpm_add_job(NULL, cb_work_incr, NULL) == NULL);
	assert(tpm_add_job(t, NULL, NULL) == NULL);

	// allocation failure while creating the job container
	alloc_reset_counter();
	alloc_set_call_limit(0);
	assert(tpm_add_job(t, cb_work_incr, NULL) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	tpm_free(t, NULL);
}

NO_COV void test_cm_tpm_add_batch(void) {
	t_cm_tpm *t = tpm_new(2);
	void     *args[5];

	for (int i = 0; i < 5; i++)
		args[i] = (void *)(uintptr_t)i;

	cb_reset_counter();
	// tpm_add_batch always returns NULL, success is observed by side effect
	assert(tpm_add_batch(t, 5, cb_work_incr, args) == NULL);
	assert(tpm_wait_end(t) == true);
	assert(cb_get_counter() == 5);

	tpm_free(t, NULL);
}

NO_COV void test_cm_tpm_add_batch_errors(void) {
	t_cm_tpm *t       = tpm_new(2);
	void     *args[3] = {NULL, NULL, NULL};

	assert(tpm_add_batch(NULL, 3, cb_work_incr, args) == NULL);
	assert(tpm_add_batch(t, 3, NULL, args) == NULL);
	assert(tpm_add_batch(t, 3, cb_work_incr, NULL) == NULL);
	assert(tpm_add_batch(t, 0, cb_work_incr, args) == NULL);

	tpm_free(t, NULL);
}
