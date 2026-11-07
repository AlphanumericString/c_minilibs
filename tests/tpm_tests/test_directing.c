#include <assert.h>
#include <unistd.h>

#include "tpm_tests.h"

// Empties and releases the main queue shortly after being started: lets a
// spin-prone wait_main_queue_empty observe its failing/regressing clock paths
// and still terminate.
NO_COV static void *drain_main_queue_later(void *arg) {
	t_cm_tpm    *t = arg;
	t_cm_tpm_jc *head;

	usleep(20000);
	pthread_mutex_lock(&t->q_lock);
	head       = t->q_start;
	t->q_start = NULL;
	t->q_end   = NULL;
	pthread_cond_signal(&t->q_cond);
	pthread_mutex_unlock(&t->q_lock);
	tpm_clear_job_list(head, NULL);
	return (NULL);
}

NO_COV void test_cm_tpm_stop(void) {
	t_cm_tpm *t = tpm_new(2);

	assert(tpm_stop(NULL) == false);
	assert(tpm_stop(t) == true);
	tpm_free(t, NULL);
}

NO_COV void test_cm_tpm_wait_end(void) {
	t_cm_tpm *t = tpm_new(2);

	assert(tpm_wait_end(NULL) == false);
	assert(tpm_wait_end(t) == true);
	tpm_free(t, NULL);
}

NO_COV void test_cm_tpm_wait_end_clock_fail(void) {
	t_cm_tpm *t = tpm_new(2);

	// make clock_gettime fail for everyone: tpm_wait_end must report it
	clock_reset_counter();
	clock_set_call_limit(0);
	assert(tpm_wait_end(t) == false);
	clock_set_call_limit(CLOCK_DFT_LIMIT);

	tpm_free(t, NULL);
}

NO_COV void test_cm_tpm_wait_end_thread_cache(void) {
	t_cm_tpm t = {0};

	// worker-less pool whose single thread cache has consumed past its end
	// (end == 0 while start != 0): wait_thread_queues_empty must treat it as
	// drained instead of spinning forever.
	t.q_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_init(&t.q_cond, NULL);
	atomic_store(&t.nb_threads, 1);
	t.threads[0].job_cache_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	t.threads[0].start          = 5;
	t.threads[0].end            = 0;

	assert(tpm_wait_end(&t) == true);

	pthread_mutex_destroy(&t.threads[0].job_cache_lock);
	pthread_cond_destroy(&t.q_cond);
	pthread_mutex_destroy(&t.q_lock);
}

NO_COV void test_cm_tpm_wait_end_clock_fail_loop(void) {
	t_cm_tpm  t = {0};
	pthread_t drain;

	t.q_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_init(&t.q_cond, NULL);
	atomic_store(&t.nb_threads, 0);
	tpmi_job_push(tpmi_create_job(cb_work_incr, NULL), &t.q_start, &t.q_end,
	              false);

	// let the entry clock_gettime through but fail every one made inside
	// wait_main_queue_empty: the deadline must stay put while the queue drains.
	clock_reset_counter();
	clock_set_call_limit(1);
	assert(pthread_create(&drain, NULL, drain_main_queue_later, &t) == 0);
	assert(tpm_wait_end(&t) == true);
	pthread_join(drain, NULL);
	clock_set_call_limit(CLOCK_DFT_LIMIT);

	pthread_cond_destroy(&t.q_cond);
	pthread_mutex_destroy(&t.q_lock);
}

NO_COV void test_cm_tpm_wait_end_clock_regress(void) {
	t_cm_tpm  t = {0};
	pthread_t drain;

	t.q_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_init(&t.q_cond, NULL);
	atomic_store(&t.nb_threads, 0);
	tpmi_job_push(tpmi_create_job(cb_work_incr, NULL), &t.q_start, &t.q_end,
	              false);

	// frozen clock: the first loop pass bumps the wait deadline ahead of the
	// (constant) samples, so the next comparison must not bump it again.
	clock_set_fake_time(0, 0);
	assert(pthread_create(&drain, NULL, drain_main_queue_later, &t) == 0);
	assert(tpm_wait_end(&t) == true);
	pthread_join(drain, NULL);
	clock_clear_fake_time();

	pthread_cond_destroy(&t.q_cond);
	pthread_mutex_destroy(&t.q_lock);
}
