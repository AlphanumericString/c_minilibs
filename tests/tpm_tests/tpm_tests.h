#pragma once

#include <stdatomic.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "common_base/tests.h"

#define C_MINI__TPM_INTERNAL
#include "tpm/thread_pool_manager.h"

// Helper callbacks / counters (callbacks.c)
void *cb_work_incr(void *arg);
void  cb_reset_counter(void);
int   cb_get_counter(void);

void cb_cleanup_count(void *arg);
void cb_reset_cleanups(void);
int  cb_get_cleanups(void);

// +----------------+
// |CREATE          |
// +----------------+
void test_tpm_init(void);
void test_tpm_new(void);
void test_tpm_new_default(void);

// +----------------+
// |ADD             |
// +----------------+
void test_tpm_add_job(void);
void test_tpm_add_job_errors(void);
void test_tpm_add_batch(void);
void test_tpm_add_batch_errors(void);

// +----------------+
// |INTERNAL JOBS   |
// +----------------+
void test_tpmi_create_job(void);
void test_tpmi_job_push(void);
void test_tpmi_job_pop(void);
void test_tpmi_job_thrd_cache_pop(void);

// +----------------+
// |DELETE          |
// +----------------+
void test_tpm_clear_job_list(void);
void test_tpm_delete(void);
void test_tpm_delete_pending(void);
void test_tpm_free(void);

// +----------------+
// |INFO            |
// +----------------+
void test_tpm_get_stats(void);
void test_tpm_get_nbthread(void);

// +----------------+
// |DIRECTING       |
// +----------------+
void test_tpm_stop(void);
void test_tpm_wait_end(void);
void test_tpm_wait_end_clock_fail(void);

// +----------------+
// |WORKER          |
// +----------------+
void test_tpmi_worker_null(void);
void test_tpmi_worker_runs_jobs(void);
void test_tpmi_worker_steals(void);
