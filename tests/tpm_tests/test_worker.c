#include <assert.h>
#include <stdatomic.h>
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
