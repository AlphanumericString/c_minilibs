#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "tpm_tests.h"

NO_COV void test_tpmi_create_job(void) {
	t_tpm_jc *j = tpmi_create_job(cb_work_incr, (void *)(uintptr_t)0x42);

	assert(j != NULL);
	assert(j->job.func == cb_work_incr);
	assert(j->job.arg == (void *)(uintptr_t)0x42);
	assert(j->job.infos.reccuring == false);
	assert(j->next == NULL);
	assert(j->prev == NULL);
	free(j);
}

NO_COV void test_tpmi_job_push(void) {
	t_tpm_jc *head = NULL;
	t_tpm_jc *tail = NULL;
	t_tpm_jc *a    = tpmi_create_job(cb_work_incr, (void *)(uintptr_t)0x1);
	t_tpm_jc *b    = tpmi_create_job(cb_work_incr, (void *)(uintptr_t)0x2);

	assert(tpmi_job_push(a, &head, &tail, false) == true);
	assert(head == a && tail == a);
	assert(a->prev == NULL && a->next == NULL);

	assert(tpmi_job_push(b, &head, &tail, false) == true);
	assert(head == b && tail == a);
	assert(b->prev == NULL && b->next == a);
	assert(a->prev == b && a->next == NULL);

	// refused when the queue is down: the list must stay untouched
	assert(tpmi_job_push(a, &head, &tail, true) == false);
	assert(head == b && tail == a);

	tpm_clear_job_list(head, NULL);
}

NO_COV void test_tpmi_job_pop(void) {
	t_tpm     t = {0};
	t_tpm_jc *a = tpmi_create_job(cb_work_incr, (void *)(uintptr_t)0x1);
	t_tpm_jc *b = tpmi_create_job(cb_work_incr, (void *)(uintptr_t)0x2);
	t_tpm_jc  r;

	// popping an empty queue yields an empty job
	r = tpmi_job_pop(&t);
	assert(r.job.func == NULL);

	tpmi_job_push(a, &t.q_start, &t.q_end, false);
	tpmi_job_push(b, &t.q_start, &t.q_end, false);

	// pops from the tail: oldest first (a) then (b); the non-recurring
	// containers are released by the pop itself.
	r = tpmi_job_pop(&t);
	assert(r.job.arg == (void *)(uintptr_t)0x1);
	assert(t.q_start == b && t.q_end == b);

	r = tpmi_job_pop(&t);
	assert(r.job.arg == (void *)(uintptr_t)0x2);
	assert(t.q_start == NULL && t.q_end == NULL);
}

NO_COV void test_tpmi_job_thrd_cache_pop(void) {
	t_tpm        t = {0};
	t_tpm_jc    *buffer[C_MINI__TPM_TRD_JOB_CACHE_LEN];
	t_tpm_jc    *jobs[C_MINI__TPM_TRD_JOB_CACHE_LEN + 2];
	const size_t cache_len = C_MINI__TPM_TRD_JOB_CACHE_LEN;
	const size_t total     = C_MINI__TPM_TRD_JOB_CACHE_LEN + 2;
	int          got;
	size_t       nb;

	// take a full cache out of a longer queue: the jobs left behind must not
	// be dropped (regression for the off-by-one that lost q_start).
	for (size_t i = 0; i < total; i++) {
		jobs[i] = tpmi_create_job(cb_work_incr, (void *)(uintptr_t)i);
		tpmi_job_push(jobs[i], &t.q_start, &t.q_end, false);
	}
	got = tpmi_job_thrd_cache_pop(&t, buffer, 0, cache_len);
	assert(got == (int)cache_len);
	nb = 0;
	for (t_tpm_jc *it = t.q_start; it; it = it->next)
		nb++;
	assert(nb == (size_t)2);
	assert((t.q_start == NULL) == (t.q_end == NULL));
	assert(t.q_start != NULL && t.q_start->prev == NULL);
	for (int i = 0; i < got; i++)
		free(buffer[i]);
	tpm_clear_job_list(t.q_start, NULL);
	t.q_start = NULL;
	t.q_end   = NULL;

	// draining the whole queue clears both ends
	for (size_t i = 0; i < cache_len; i++) {
		jobs[i] = tpmi_create_job(cb_work_incr, NULL);
		tpmi_job_push(jobs[i], &t.q_start, &t.q_end, false);
	}
	got = tpmi_job_thrd_cache_pop(&t, buffer, 0, cache_len);
	assert(got == (int)cache_len);
	assert(t.q_start == NULL && t.q_end == NULL);
	for (int i = 0; i < got; i++)
		free(buffer[i]);

	// bounded take: only `max` jobs leave the queue
	for (int i = 0; i < 3; i++) {
		jobs[i] = tpmi_create_job(cb_work_incr, NULL);
		tpmi_job_push(jobs[i], &t.q_start, &t.q_end, false);
	}
	got = tpmi_job_thrd_cache_pop(&t, buffer, 0, 2);
	assert(got == 2);
	assert(t.q_start != NULL && t.q_end != NULL);
	for (int i = 0; i < got; i++)
		free(buffer[i]);
	tpm_clear_job_list(t.q_start, NULL);
	t.q_start = NULL;
	t.q_end   = NULL;

	// bad arguments
	assert(tpmi_job_thrd_cache_pop(NULL, buffer, 0, 1) == -1);
}
