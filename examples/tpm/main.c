#include <pthread.h>
#include <stdatomic.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#include "tpm/thread_pool_manager.h"

static atomic_int g_completed;

static void *heavy_job(void *arg) {
	long millis = (long)(intptr_t)arg;

	usleep((useconds_t)(millis * 1000));
	atomic_fetch_add(&g_completed, 1);
	return (NULL);
}

int main(void) {
	const size_t threads = 4;
	const size_t jobs    = 24;
	t_tpm       *pool    = tpm_new(threads);

	printf("pool started with %zu worker threads\n", tpm_get_nbthread(pool));

	for (size_t i = 0; i < jobs; i++)
		tpm_add_job(pool, heavy_job, (void *)(intptr_t)(1 + (i % 4)));

	void *args[4] = {(void *)(intptr_t)1, (void *)(intptr_t)1,
	                 (void *)(intptr_t)1, (void *)(intptr_t)1};
	tpm_add_batch(pool, 4, heavy_job, args);

	printf("dispatched %zu jobs, waiting for completion...\n", jobs + 4);
	tpm_wait_end(pool);
	printf("completed jobs: %d\n", atomic_load(&g_completed));

	tpm_free(pool, NULL);
	return (0);
}
