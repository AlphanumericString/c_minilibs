#include <stdlib.h>

#define C_MINI__TWRAP_IMPL
#include "tpm_tests.h"

NO_COV int main(void) {
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Create", )
	test_cm_tpm_init();
	test_cm_tpm_new();
	test_cm_tpm_new_default();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Add", )
	test_cm_tpm_add_job();
	test_cm_tpm_add_job_errors();
	test_cm_tpm_add_batch();
	test_cm_tpm_add_batch_errors();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Jobs", )
	test_cm_tpmi_create_job();
	test_cm_tpmi_job_push();
	test_cm_tpmi_job_pop();
	test_cm_tpmi_job_thrd_cache_pop();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Delete", )
	test_cm_tpm_clear_job_list();
	test_cm_tpm_delete();
	test_cm_tpm_delete_pending();
	test_cm_tpm_free();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Info", )
	test_cm_tpm_get_stats();
	test_cm_tpm_get_nbthread();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Directing", )
	test_cm_tpm_stop();
	test_cm_tpm_wait_end();
	test_cm_tpm_wait_end_clock_fail();
	CM_LOG(C_MINI__LEVEL_ALWAYS, "", )
	CM_LOG(C_MINI__LEVEL_ALWAYS, "Worker", )
	test_cm_tpmi_worker_null();
	test_cm_tpmi_worker_runs_jobs();
	test_cm_tpmi_worker_steals();

	CM_LOG(C_MINI__LEVEL_ALWAYS, "All tpm tests completed successfully.", )
	return (EXIT_SUCCESS);
}
