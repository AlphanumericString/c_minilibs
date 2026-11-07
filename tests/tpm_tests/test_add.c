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

	// allocation failure while building the chain: the partial batch built so
	// far must be released and nothing left behind (the failure happens before
	// anything reaches the shared queue).
	alloc_reset_counter();
	alloc_set_call_limit(1);
	assert(tpm_add_batch(t, 3, cb_work_incr, args) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	tpm_free(t, NULL);
}

NO_COV void test_cm_tpm_add_batch_shutdown(void) {
	t_cm_tpm *t       = tpm_new(2);
	void     *args[2] = {NULL, NULL};

	// once the pool is shutting down a batch is rejected outright
	assert(tpm_stop(t) == true);
	assert(tpm_add_batch(t, 2, cb_work_incr, args) == NULL);

	tpm_free(t, NULL);
}

NO_COV void test_cm_tpm_add_batch_append(void) {
	t_cm_tpm t = {0};

	// queue-backed pool without workers so we can observe the list splicing
	t.q_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_init(&t.q_cond, NULL);
	atomic_store(&t.nb_threads, 0);

	tpmi_job_push(tpmi_create_job(cb_work_incr, NULL), &t.q_start, &t.q_end,
	              false);

	// prepending a batch onto a non-empty queue must keep both ends coherent
	{
		void *args[2] = {NULL, NULL};

		assert(tpm_add_batch(&t, 2, cb_work_incr, args) == NULL);
	}
	assert(t.q_start != NULL && t.q_end != NULL);

	tpm_clear_job_list(t.q_start, NULL);
	pthread_cond_destroy(&t.q_cond);
	pthread_mutex_destroy(&t.q_lock);
}
