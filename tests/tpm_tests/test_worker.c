#include <assert.h>
#include <stdatomic.h>
#include <stdlib.h>
#include <unistd.h>

#include "tpm_tests.h"

static atomic_int w_concurrent;
static atomic_int w_max_concurrent;
static atomic_int w_release;
static atomic_int w_done;

// Blocks from the moment it starts until the main thread releases the gate.
// Without stealing only the single worker that owns the batch can run one, so
// the observed concurrency stays at 1.
NO_COV static void *w_blocking(void *arg) {
	int c;
	int seen;

	(void)arg;
	c    = atomic_fetch_add(&w_concurrent, 1) + 1;
	seen = atomic_load(&w_max_concurrent);
	while (c > seen &&
	       !atomic_compare_exchange_weak(&w_max_concurrent, &seen, c))
		;
	while (!atomic_load(&w_release))
		usleep(1000);
	atomic_fetch_sub(&w_concurrent, 1);
	atomic_fetch_add(&w_done, 1);
	return (NULL);
}

NO_COV void test_cm_tpmi_worker_null(void) {
	assert(tpmi_worker(NULL) == NULL);
}

NO_COV void test_cm_tpmi_worker_runs_jobs(void) {
	t_cm_tpm *t = tpm_new(4);

	cb_reset_counter();
	for (int i = 0; i < 64; i++)
		tpm_add_job(t, cb_work_incr, NULL);
	assert(tpm_wait_end(t) == true);
	assert(cb_get_counter() == 64);

	tpm_free(t, NULL);
}

NO_COV void test_cm_tpmi_worker_steals(void) {
	const size_t nb = C_MINI__TPM_TRD_MGET_NB;
	t_cm_tpm    *t  = tpm_new(3);
	void        *args[C_MINI__TPM_TRD_MGET_NB];
	int          spins = 0;

	atomic_store(&w_concurrent, 0);
	atomic_store(&w_max_concurrent, 0);
	atomic_store(&w_release, 0);
	atomic_store(&w_done, 0);

	// one atomic splice so a single worker owns the whole batch, starts the
	// first job and blocks there; the other workers must steal the rest.
	for (size_t i = 0; i < nb; i++)
		args[i] = NULL;
	tpm_add_batch(t, nb, w_blocking, args);

	while (atomic_load(&w_max_concurrent) <= 1 && spins++ < 500)
		usleep(2000);

	// at least one idle worker had to steal a job to run concurrently
	assert(atomic_load(&w_max_concurrent) > 1);

	atomic_store(&w_release, 1);
	assert(tpm_wait_end(t) == true);
	assert(atomic_load(&w_done) == (int)nb);

	tpm_free(t, NULL);
}

// -- deterministic worker-loop drivers -------------------------------------
// A live worker is launched on a hand-built pool so the internal get_new_jobs
// / steal_work branches can be exercised without racing the scheduler.

NO_COV static void fake_pool_init(t_cm_tpm *t, size_t nb) {
	*t        = (t_cm_tpm){0};
	t->q_lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	pthread_cond_init(&t->q_cond, NULL);
	atomic_store(&t->nb_threads, nb);
	for (size_t i = 0; i < nb; i++) {
		t->threads[i].pool          = t;
		t->threads[i].id_sequential = i;
		t->threads[i].job_cache_lock =
		    (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
	}
}

NO_COV static void fake_pool_stop_join(t_cm_tpm *t, pthread_t w) {
	pthread_mutex_lock(&t->q_lock);
	t->internal_val.shutting_down = true;
	pthread_mutex_unlock(&t->q_lock);
	pthread_cond_broadcast(&t->q_cond);
	pthread_join(w, NULL);
}

NO_COV static void fake_pool_destroy(t_cm_tpm *t, size_t nb) {
	for (size_t i = 0; i < nb; i++)
		pthread_mutex_destroy(&t->threads[i].job_cache_lock);
	pthread_cond_destroy(&t->q_cond);
	pthread_mutex_destroy(&t->q_lock);
}

NO_COV void test_cm_tpmi_worker_single(void) {
	// a lone worker always fails the steal_work early "nb <= 1" guard
	t_cm_tpm *t = tpm_new(1);

	cb_reset_counter();
	for (int i = 0; i < 8; i++)
		tpm_add_job(t, cb_work_incr, NULL);
	assert(tpm_wait_end(t) == true);
	assert(cb_get_counter() == 8);

	tpm_free(t, NULL);
}

NO_COV void test_cm_tpmi_worker_wide(void) {
	// more workers than the steal window: MIN() must clamp to the window
	t_cm_tpm *t = tpm_new(C_MINI__TPM_STEAL_WINDOW + 4);

	cb_reset_counter();
	for (int i = 0; i < 128; i++)
		tpm_add_job(t, cb_work_incr, NULL);
	assert(tpm_wait_end(t) == true);
	assert(cb_get_counter() == 128);

	tpm_free(t, NULL);
}

NO_COV void test_cm_tpmi_worker_held_lock(void) {
	t_cm_tpm  t;
	pthread_t w;

	fake_pool_init(&t, 2);
	// hold the neighbour's cache lock for the whole run: the worker's trylock
	// has to give up and move on instead of blocking.
	pthread_mutex_lock(&t.threads[1].job_cache_lock);

	tpmi_job_push(tpmi_create_job(cb_work_incr, NULL), &t.q_start, &t.q_end,
	              false);

	cb_reset_counter();
	assert(pthread_create(&w, NULL, tpmi_worker, &t.threads[0]) == 0);
	while (cb_get_counter() < 1)
		usleep(1000);
	fake_pool_stop_join(&t, w);

	pthread_mutex_unlock(&t.threads[1].job_cache_lock);
	tpm_clear_job_list(t.q_start, NULL);
	fake_pool_destroy(&t, 2);
}

NO_COV void test_cm_tpmi_worker_steal_capped(void) {
	t_cm_tpm  t;
	pthread_t w;

	fake_pool_init(&t, 3);
	// neighbour cache is full so half of it would overflow the worker's
	// free space once it has already taken its main-queue slice.
	t.threads[1].start = 0;
	t.threads[1].end   = C_MINI__TPM_TRD_JOB_CACHE_LEN;
	for (size_t i = 0; i < C_MINI__TPM_TRD_JOB_CACHE_LEN; i++)
		t.threads[1].jc[i] = tpmi_create_job(cb_work_incr, NULL);

	for (size_t i = 0; i < C_MINI__TPM_TRD_MGET_NB; i++)
		tpmi_job_push(tpmi_create_job(cb_work_incr, NULL), &t.q_start, &t.q_end,
		              false);

	cb_reset_counter();
	assert(pthread_create(&w, NULL, tpmi_worker, &t.threads[0]) == 0);
	while (cb_get_counter() < 1)
		usleep(1000);
	fake_pool_stop_join(&t, w);

	// anything still owned by the neighbour cache (the stolen tail was moved
	// into the worker cache and consumed there) still needs releasing.
	for (size_t i = 0; i < t.threads[1].end; i++)
		free(t.threads[1].jc[i]);
	tpm_clear_job_list(t.q_start, NULL);
	fake_pool_destroy(&t, 3);
}

NO_COV void test_cm_tpmi_worker_steal_skip_small(void) {
	t_cm_tpm  t;
	pthread_t w;

	fake_pool_init(&t, 2);
	// neighbour holds more than one job but fewer than the worker already
	// holds after its main-queue slice, so it is not worth stealing from.
	t.threads[1].start = 0;
	t.threads[1].end   = 3;
	for (size_t i = 0; i < 3; i++)
		t.threads[1].jc[i] = tpmi_create_job(cb_work_incr, NULL);

	for (size_t i = 0; i < C_MINI__TPM_TRD_MGET_NB; i++)
		tpmi_job_push(tpmi_create_job(cb_work_incr, NULL), &t.q_start, &t.q_end,
		              false);

	cb_reset_counter();
	assert(pthread_create(&w, NULL, tpmi_worker, &t.threads[0]) == 0);
	while (cb_get_counter() < 1)
		usleep(1000);
	fake_pool_stop_join(&t, w);

	for (size_t i = 0; i < t.threads[1].end; i++)
		free(t.threads[1].jc[i]);
	tpm_clear_job_list(t.q_start, NULL);
	fake_pool_destroy(&t, 2);
}

NO_COV void test_cm_tpmi_worker_preloaded(void) {
	t_cm_tpm  t;
	pthread_t w;

	fake_pool_init(&t, 1);
	// worker already owns work: get_new_jobs must short-circuit and run it
	// rather than touching the main queue.
	t.threads[0].start = 0;
	t.threads[0].end   = 2;
	t.threads[0].jc[0] = tpmi_create_job(cb_work_incr, NULL);
	t.threads[0].jc[1] = tpmi_create_job(cb_work_incr, NULL);

	tpmi_job_push(tpmi_create_job(cb_work_incr, NULL), &t.q_start, &t.q_end,
	              false);

	cb_reset_counter();
	assert(pthread_create(&w, NULL, tpmi_worker, &t.threads[0]) == 0);
	while (cb_get_counter() < 1)
		usleep(1000);
	fake_pool_stop_join(&t, w);

	// the preloaded pair must have run to completion
	assert(cb_get_counter() >= 2);
	tpm_clear_job_list(t.q_start, NULL);
	fake_pool_destroy(&t, 1);
}

NO_COV void test_cm_tpmi_worker_wait_clock_fail(void) {
	t_cm_tpm *t = tpm_new(1);

	// let the lone worker settle into its timed wait
	usleep(10000);

	// every following clock_gettime fails: the idle worker has to fall back
	// to an untimed condition wait.
	clock_reset_counter();
	clock_set_call_limit(0);
	usleep(20000);

	cb_reset_counter();
	tpm_add_job(t, cb_work_incr, NULL);
	clock_set_call_limit(CLOCK_DFT_LIMIT);

	assert(tpm_wait_end(t) == true);
	assert(cb_get_counter() == 1);

	tpm_free(t, NULL);
}

NO_COV void test_cm_tpmi_worker_wait_clock_overflow(void) {
	// frozen clock one nanosecond short of a second: adding the retry delay
	// must carry into tv_sec instead of overflowing tv_nsec.
	clock_set_fake_time(0, 999999999L);
	t_cm_tpm *t = tpm_new(1);
	usleep(5000);
	clock_clear_fake_time();

	cb_reset_counter();
	tpm_add_job(t, cb_work_incr, NULL);
	assert(tpm_wait_end(t) == true);
	assert(cb_get_counter() == 1);

	tpm_free(t, NULL);
}
