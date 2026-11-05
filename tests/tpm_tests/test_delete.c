#include <assert.h>
#include <pthread.h>
#include <stdint.h>

#include "tpm_tests.h"

NO_COV void test_cm_tpm_clear_job_list(void) {
	t_cm_tpm_jc *a = tpmi_create_job(cb_work_incr, (void *)(uintptr_t)0x1);
	t_cm_tpm_jc *b = tpmi_create_job(cb_work_incr, (void *)(uintptr_t)0x2);
	t_cm_tpm_jc *c = tpmi_create_job(cb_work_incr, (void *)(uintptr_t)0x3);

	a->next = b;
	b->prev = a;
	b->next = c;
	c->prev = b;

	cb_reset_cleanups();
	tpm_clear_job_list(a, cb_cleanup_count);
	assert(cb_get_cleanups() == 3);

	// a NULL head is a no-op, with or without cleanup
	tpm_clear_job_list(NULL, cb_cleanup_count);
	assert(cb_get_cleanups() == 3);
}

NO_COV void test_cm_tpm_delete(void) {
	t_cm_tpm t;

	tpm_init(&t, 2);
	cb_reset_counter();
	assert(tpm_add_job(&t, cb_work_incr, NULL) != NULL);
	assert(tpm_add_job(&t, cb_work_incr, NULL) != NULL);
	assert(tpm_wait_end(&t) == true);
	assert(cb_get_counter() == 2);
	tpm_delete(&t, NULL);

	tpm_delete(NULL, NULL);
}

NO_COV void test_cm_tpm_delete_pending(void) {
	t_cm_tpm t = {0};

	// a pool with no worker threads: the whole main queue is dropped and each
	// job's cleanup runs exactly once (guards the former double-free of the
	// main queue).
	t.q_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_init(&t.q_cond, NULL);
	atomic_store(&t.nb_threads, 0);

	cb_reset_cleanups();
	for (int i = 0; i < 3; i++)
		tpmi_job_push(tpmi_create_job(cb_work_incr, (void *)(uintptr_t)0x1),
		              &t.q_start, &t.q_end, false);
	tpm_delete(&t, cb_cleanup_count);
	assert(cb_get_cleanups() == 3);

	pthread_cond_destroy(&t.q_cond);
	pthread_mutex_destroy(&t.q_lock);
}

NO_COV void test_cm_tpm_free(void) {
	t_cm_tpm *t = tpm_new(2);

	cb_reset_counter();
	assert(tpm_add_job(t, cb_work_incr, NULL) != NULL);
	assert(tpm_wait_end(t) == true);
	assert(cb_get_counter() == 1);
	tpm_free(t, NULL);

	tpm_free(NULL, NULL);
}
