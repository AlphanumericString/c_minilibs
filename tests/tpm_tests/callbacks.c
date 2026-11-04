#include <assert.h>
#include <stdlib.h>

#include "tpm_tests.h"

static atomic_int g_counter;
static atomic_int g_cleanups;

// Job body: records that it ran. Ignores its argument.
void *cb_work_incr(void *arg) {
	(void)arg;
	atomic_fetch_add(&g_counter, 1);
	return (NULL);
}
void cb_reset_counter(void) {
	atomic_store(&g_counter, 0);
}
int cb_get_counter(void) {
	return (atomic_load(&g_counter));
}

// Cleanup callback: records that it was called. Ignores its argument.
void cb_cleanup_count(void *arg) {
	(void)arg;
	atomic_fetch_add(&g_cleanups, 1);
}
void cb_reset_cleanups(void) {
	atomic_store(&g_cleanups, 0);
}
int cb_get_cleanups(void) {
	return (atomic_load(&g_cleanups));
}
