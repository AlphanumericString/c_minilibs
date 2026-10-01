#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

#include "./tpm/thread_pool_manager.h"

/* Example 1: Simple job processing */

// #include "tpm/claudius/threadpool.h"

typedef struct {
	int id;
	int value;
} work_item_t;

void *process_work(void *arg) {
	work_item_t *item         = (work_item_t *)arg;
	const size_t one_s_in_ms  = 1000;
	const size_t one_ms_in_us = 1000; // 1ms;
	size_t       us_wait_time = 0;

	/* Simulate work randomness */
	if (item->id % 2) us_wait_time = 100 * one_ms_in_us;      // .10s
	else if (item->id % 3) us_wait_time = 300 * one_ms_in_us; // .30s
	else us_wait_time = 10 * one_ms_in_us;                    // .01s
	CM_LOG(C_MINI__LEVEL_ALWAYS,
	       "[Worker] Processing item (%p) %02d (value=%d) {estimate:%zums "
	       "(%.2f s)}",
	       item, item->id, item->value, us_wait_time / one_ms_in_us,
	       (float)us_wait_time / (one_s_in_ms * one_ms_in_us));

	usleep(us_wait_time);

	CM_LOG(C_MINI__LEVEL_ALWAYS, "[Worker] Completed item %02d", item->id);
	free(item);
	return (NULL);
}

int main(int ac, char **av) {
	t_tpm    *t        = tpm_new(4);
	const int nb_items = ac > 1 ? atoi(av[1]) : 20;
	// work_item_t *item_batch[20];

	for (int i = 0; i < nb_items; i++) {
		work_item_t *item = malloc(sizeof(work_item_t));
		if (!item) {
			CM_DBG("Memory allocation failed\n");
			break;
		}
		item->id    = i;
		item->value = i * 100;
		CM_DBG("MARKER: ITEM_ALLOC: %p %d", item, item->id);
		tpm_add_job(t, process_work, item);
	}
	tpm_wait_end(t);

	// CM_LOG(C_MINI__LEVEL_ALWAYS, "===========");
	// CM_LOG(C_MINI__LEVEL_ALWAYS, "===========");
	// CM_LOG(C_MINI__LEVEL_ALWAYS, "===========");
	// CM_LOG(C_MINI__LEVEL_ALWAYS, "marker here");
	// CM_LOG(C_MINI__LEVEL_ALWAYS, "===========");
	// CM_LOG(C_MINI__LEVEL_ALWAYS, "===========");
	// CM_LOG(C_MINI__LEVEL_ALWAYS, "===========");
	// for (int i = 0; i < nb_items; i++) {
	// 	item_batch[i] = malloc(sizeof(work_item_t));
	// 	if (!item_batch[i]) {
	// 		CM_DBG("Memory allocation failed\n");
	// 		break;
	// 	}
	// 	item_batch[i]->id    = i + 20;
	// 	item_batch[i]->value = (i + 20) * 100;
	// }
	// tpm_add_batch(t, 20, process_work, (void **)item_batch);
	// tpm_wait_end(t);

	tpm_free(t, free);
	return 0;
}

//
// void example_simple(void) {
// 	printf("=== Example 1: Simple Job Processing ===\n\n");
//
// 	/* Step 1: Create threads with custom attributes */
// 	int            num_threads = 4;
// 	pthread_t      threads[num_threads];
// 	pthread_attr_t attr;
//
// 	pthread_attr_init(&attr);
// 	pthread_attr_setstacksize(&attr, 256 * 1024); /* 256KB stack */
//
// 	for (int i = 0; i < num_threads; i++) {
// 		if (pthread_create(&threads[i], &attr, tp_worker, NULL) != 0) {
// 			fprintf(stderr, "Failed to create thread %d\n", i);
// 			pthread_attr_destroy(&attr);
// 			return;
// 		}
// 	}
// 	pthread_attr_destroy(&attr);
//
// 	/* Step 2: Create pool */
// 	threadpool_t *tp = tp_create_ex(threads, num_threads);
// 	if (!tp) {
// 		fprintf(stderr, "Failed to create threadpool\n");
// 		return;
// 	}
//
// 	printf("Created pool with %d threads\n\n", num_threads);
//
// 	/* Step 3: Submit work */
// 	printf("Submitting 20 jobs...\n");
// 	for (int i = 0; i < 20; i++) {
// 		work_item_t *item = malloc(sizeof(work_item_t));
// 		if (!item) {
// 			fprintf(stderr, "Memory allocation failed\n");
// 			break;
// 		}
// 		item->id    = i;
// 		item->value = i * 100;
//
// 		int ret = tp_enqueue(tp, process_work, item);
// 		if (ret != TP_OK) {
// 			fprintf(stderr, "Enqueue failed for job %d: %d\n", i, ret);
// 			free(item);
// 		}
// 	}
//
// 	printf("Queue size: %d\n\n", tp_queue_size(tp));
//
// 	/* Step 4: Wait for completion with timeout */
// 	printf("Waiting for jobs (5 second timeout)...\n");
// 	int completed = tp_shutdown(tp, 5000);
// 	printf("Shutdown complete. Jobs completed: %d\n\n", completed);
//
// 	/* Step 5: Cleanup */
// 	tp_destroy(tp);
//
// 	/* Step 6: Join threads (user responsibility) */
// 	for (int i = 0; i < num_threads; i++) {
// 		pthread_join(threads[i], NULL);
// 	}
//
// 	printf("Example 1 complete.\n\n");
// }
//
// /* Example 2: Error handling */
//
// void dummy_job(void *arg) {
// 	/* Do nothing */
// }
//
// void example_error_handling(void) {
// 	printf("=== Example 2: Error Handling ===\n\n");
//
// 	pthread_t      threads[2];
// 	pthread_attr_t attr;
// 	pthread_attr_init(&attr);
//
// 	for (int i = 0; i < 2; i++) {
// 		pthread_create(&threads[i], &attr, tp_worker, NULL);
// 	}
// 	pthread_attr_destroy(&attr);
//
// 	threadpool_t *tp = tp_create_ex(threads, 2);
// 	if (!tp) {
// 		fprintf(stderr, "Failed to create threadpool\n");
// 		return;
// 	}
//
// 	printf("Testing error conditions:\n\n");
//
// 	/* Test NULL check */
// 	printf("1. Passing NULL tp: %s\n",
// 	       (tp_enqueue(NULL, dummy_job, NULL) == TP_ERR_NULL) ? "CAUGHT"
// 	                                                          : "MISSED");
//
// 	/* Test NULL function pointer */
// 	printf("2. Passing NULL func: %s\n",
// 	       (tp_enqueue(tp, NULL, NULL) == TP_ERR_NULL) ? "CAUGHT" : "MISSED");
//
// 	/* Fill the queue and test TP_ERR_QUEUE */
// 	printf("3. Filling queue (max size=%d)...\n", TP_MAX_QUEUE_SIZE);
// 	int queued = 0;
// 	for (int i = 0; i < TP_MAX_QUEUE_SIZE + 10; i++) {
// 		int ret = tp_enqueue(tp, dummy_job, NULL);
// 		if (ret == TP_OK) {
// 			queued++;
// 		} else if (ret == TP_ERR_QUEUE) {
// 			printf("   Queue full after %d jobs\n", queued);
// 			break;
// 		}
// 	}
//
// 	/* Shutdown and test TP_ERR_SHUTDOWN */
// 	printf("4. Shutting down pool...\n");
// 	tp_shutdown(tp, 0); /* Don't wait */
//
// 	printf("5. Trying to enqueue during shutdown: %s\n",
// 	       (tp_enqueue(tp, dummy_job, NULL) == TP_ERR_SHUTDOWN) ? "CAUGHT"
// 	                                                            : "MISSED");
//
// 	tp_destroy(tp);
//
// 	/* Don't bother joining in this example */
// 	printf("\nExample 2 complete.\n\n");
// }
//
// /* Example 3: Demonstrating timeout */
//
// void slow_job(void *arg) {
// 	int job_id = (intptr_t)arg;
// 	printf("Job %d: sleeping for 2 seconds...\n", job_id);
// 	sleep(2);
// 	printf("Job %d: done\n", job_id);
// }
//
// void example_timeout(void) {
// 	printf("=== Example 3: Shutdown Timeout ===\n\n");
//
// 	pthread_t      threads[2];
// 	pthread_attr_t attr;
// 	pthread_attr_init(&attr);
//
// 	for (int i = 0; i < 2; i++) {
// 		pthread_create(&threads[i], &attr, tp_worker, NULL);
// 	}
// 	pthread_attr_destroy(&attr);
//
// 	threadpool_t *tp = tp_create_ex(threads, 2);
// 	if (!tp) {
// 		fprintf(stderr, "Failed to create threadpool\n");
// 		return;
// 	}
//
// 	/* Queue several slow jobs */
// 	printf("Queueing 5 slow jobs (2 seconds each)...\n");
// 	for (int i = 0; i < 5; i++) {
// 		tp_enqueue(tp, slow_job, (void *)(intptr_t)i);
// 	}
//
// 	printf("Initiating shutdown with 3-second timeout...\n");
// 	printf("(With 2 threads and 5 jobs of 2 seconds each, not all will "
// 	       "complete)\n\n");
//
// 	time_t start     = time(NULL);
// 	int    completed = tp_shutdown(tp, 3000); /* 3 second timeout */
// 	time_t elapsed   = time(NULL) - start;
//
// 	printf("Shutdown completed in %ld seconds\n", elapsed);
// 	printf("Jobs completed: %d / 5\n\n", completed);
//
// 	tp_destroy(tp);
//
// 	printf("Example 3 complete.\n\n");
// }
//
