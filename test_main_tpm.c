#include <stdio.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#include "tpm/threadpool.h"

/* Example 1: Simple job processing */

typedef struct {
	int id;
	int value;
} work_item_t;

void process_work(void *arg) {
	work_item_t *item = (work_item_t *)arg;
	printf("[Worker] Processing item %d (value=%d)\n", item->id, item->value);

	/* Simulate work */
	usleep(100000); /* 100ms */

	printf("[Worker] Completed item %d\n", item->id);
	free(item);
}

void example_simple(void) {
	printf("=== Example 1: Simple Job Processing ===\n\n");

	/* Step 1: Create threads with custom attributes */
	int            num_threads = 4;
	pthread_t      threads[num_threads];
	pthread_attr_t attr;

	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, 256 * 1024); /* 256KB stack */

	for (int i = 0; i < num_threads; i++) {
		if (pthread_create(&threads[i], &attr, tp_worker, NULL) != 0) {
			fprintf(stderr, "Failed to create thread %d\n", i);
			pthread_attr_destroy(&attr);
			return;
		}
	}
	pthread_attr_destroy(&attr);

	/* Step 2: Create pool */
	threadpool_t *tp = tp_create_ex(threads, num_threads);
	if (!tp) {
		fprintf(stderr, "Failed to create threadpool\n");
		return;
	}

	printf("Created pool with %d threads\n\n", num_threads);

	/* Step 3: Submit work */
	printf("Submitting 20 jobs...\n");
	for (int i = 0; i < 20; i++) {
		work_item_t *item = malloc(sizeof(work_item_t));
		if (!item) {
			fprintf(stderr, "Memory allocation failed\n");
			break;
		}
		item->id    = i;
		item->value = i * 100;

		int ret = tp_enqueue(tp, process_work, item);
		if (ret != TP_OK) {
			fprintf(stderr, "Enqueue failed for job %d: %d\n", i, ret);
			free(item);
		}
	}

	printf("Queue size: %d\n\n", tp_queue_size(tp));

	/* Step 4: Wait for completion with timeout */
	printf("Waiting for jobs (5 second timeout)...\n");
	int completed = tp_shutdown(tp, 5000);
	printf("Shutdown complete. Jobs completed: %d\n\n", completed);

	/* Step 5: Cleanup */
	tp_destroy(tp);

	/* Step 6: Join threads (user responsibility) */
	for (int i = 0; i < num_threads; i++) {
		pthread_join(threads[i], NULL);
	}

	printf("Example 1 complete.\n\n");
}

/* Example 2: Error handling */

void dummy_job(void *arg) {
	/* Do nothing */
}

void example_error_handling(void) {
	printf("=== Example 2: Error Handling ===\n\n");

	pthread_t      threads[2];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	for (int i = 0; i < 2; i++) {
		pthread_create(&threads[i], &attr, tp_worker, NULL);
	}
	pthread_attr_destroy(&attr);

	threadpool_t *tp = tp_create_ex(threads, 2);
	if (!tp) {
		fprintf(stderr, "Failed to create threadpool\n");
		return;
	}

	printf("Testing error conditions:\n\n");

	/* Test NULL check */
	printf("1. Passing NULL tp: %s\n",
	       (tp_enqueue(NULL, dummy_job, NULL) == TP_ERR_NULL) ? "CAUGHT"
	                                                          : "MISSED");

	/* Test NULL function pointer */
	printf("2. Passing NULL func: %s\n",
	       (tp_enqueue(tp, NULL, NULL) == TP_ERR_NULL) ? "CAUGHT" : "MISSED");

	/* Fill the queue and test TP_ERR_QUEUE */
	printf("3. Filling queue (max size=%d)...\n", TP_MAX_QUEUE_SIZE);
	int queued = 0;
	for (int i = 0; i < TP_MAX_QUEUE_SIZE + 10; i++) {
		int ret = tp_enqueue(tp, dummy_job, NULL);
		if (ret == TP_OK) {
			queued++;
		} else if (ret == TP_ERR_QUEUE) {
			printf("   Queue full after %d jobs\n", queued);
			break;
		}
	}

	/* Shutdown and test TP_ERR_SHUTDOWN */
	printf("4. Shutting down pool...\n");
	tp_shutdown(tp, 0); /* Don't wait */

	printf("5. Trying to enqueue during shutdown: %s\n",
	       (tp_enqueue(tp, dummy_job, NULL) == TP_ERR_SHUTDOWN) ? "CAUGHT"
	                                                            : "MISSED");

	tp_destroy(tp);

	/* Don't bother joining in this example */
	printf("\nExample 2 complete.\n\n");
}

/* Example 3: Demonstrating timeout */

void slow_job(void *arg) {
	int job_id = (intptr_t)arg;
	printf("Job %d: sleeping for 2 seconds...\n", job_id);
	sleep(2);
	printf("Job %d: done\n", job_id);
}

void example_timeout(void) {
	printf("=== Example 3: Shutdown Timeout ===\n\n");

	pthread_t      threads[2];
	pthread_attr_t attr;
	pthread_attr_init(&attr);

	for (int i = 0; i < 2; i++) {
		pthread_create(&threads[i], &attr, tp_worker, NULL);
	}
	pthread_attr_destroy(&attr);

	threadpool_t *tp = tp_create_ex(threads, 2);
	if (!tp) {
		fprintf(stderr, "Failed to create threadpool\n");
		return;
	}

	/* Queue several slow jobs */
	printf("Queueing 5 slow jobs (2 seconds each)...\n");
	for (int i = 0; i < 5; i++) {
		tp_enqueue(tp, slow_job, (void *)(intptr_t)i);
	}

	printf("Initiating shutdown with 3-second timeout...\n");
	printf("(With 2 threads and 5 jobs of 2 seconds each, not all will "
	       "complete)\n\n");

	time_t start     = time(NULL);
	int    completed = tp_shutdown(tp, 3000); /* 3 second timeout */
	time_t elapsed   = time(NULL) - start;

	printf("Shutdown completed in %ld seconds\n", elapsed);
	printf("Jobs completed: %d / 5\n\n", completed);

	tp_destroy(tp);

	printf("Example 3 complete.\n\n");
}

int main(void) {
	printf("Threadpool Library Examples\n");
	printf("=============================\n\n");

	example_simple();
	example_error_handling();
	example_timeout();

	printf("All examples complete.\n");
	return 0;
}
