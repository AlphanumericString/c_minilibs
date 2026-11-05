#include <assert.h>

#include "tpm_tests.h"

NO_COV void test_cm_tpm_get_stats(void) {
	t_cm_tpm             *t = tpm_new(2);
	struct s_cm_tpm_stats s = tpm_get_stats(t);

	assert(s.total_jobs_completed == 0);
	assert(s.total_jobs_failed == 0);
	tpm_free(t, NULL);

	// NULL pool reports an error through both counters
	s = tpm_get_stats(NULL);
	assert(s.total_jobs_completed == (size_t)-1);
	assert(s.total_jobs_failed == (size_t)-1);
}

NO_COV void test_cm_tpm_get_nbthread(void) {
	t_cm_tpm *t = tpm_new(4);

	assert(tpm_get_nbthread(t) == 4);
	tpm_free(t, NULL);
	assert(tpm_get_nbthread(NULL) == 0);
}
