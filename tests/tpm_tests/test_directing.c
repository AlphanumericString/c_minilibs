#include <assert.h>

#include "tpm_tests.h"

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
