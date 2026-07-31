#include "threadpool.h"

// #include <stdbool.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
//
// /* ============================================================================
//  * Worker Thread Function
//  * ========================================================================== */
//
// /**
//  * tp_worker - Worker thread main loop
//  *
//  * This is called by user via pthread_create().
//  * Arg is actually a threadpool_t*, not a plain void*.
//  */
// void *tp_worker(void *arg) {
// 	threadpool_t *tp = (threadpool_t *)arg;
//
// 	while (1) {
// 		job_t job     = {0};
// 		bool  got_job = false;
//
// 		/* Acquire lock and wait for job or shutdown signal */
// 		pthread_mutex_lock(&tp->queue_lock);
// 		{
// 			while (tp->queue.count == 0 && !tp->shutting_down) {
// 				pthread_cond_wait(&tp->queue_cond, &tp->queue_lock);
// 			}
//
// 			/* If queue has jobs, dequeue */
// 			if (tp->queue.count > 0) {
// 				job            = tp->queue.jobs[tp->queue.tail];
// 				tp->queue.tail = (tp->queue.tail + 1) % TP_MAX_QUEUE_SIZE;
// 				tp->queue.count--;
// 				got_job = true;
// 			}
//
// 			/* If shutting down and queue is empty, exit */
// 			if (tp->shutting_down && tp->queue.count == 0) {
// 				pthread_mutex_unlock(&tp->queue_lock);
// 				break; /* Exit thread */
// 			}
// 		}
// 		pthread_mutex_unlock(&tp->queue_lock);
//
// 		/* Execute job outside lock */
// 		if (got_job && job.func) {
// 			job.func(job.arg);
//
// 			/* Update completed count */
// 			pthread_mutex_lock(&tp->queue_lock);
// 			{
// 				tp->jobs_completed++;
// 			}
// 			pthread_mutex_unlock(&tp->queue_lock);
// 		}
// 	}
//
// 	return NULL;
// }
//
// /* ============================================================================
//  * Public API Implementation
//  * ========================================================================== */
//
// threadpool_t *tp_create_ex(pthread_t *thread_ids, unsigned num_threads) {
// 	if (!thread_ids || num_threads == 0) {
// 		return NULL;
// 	}
//
// 	/* Allocate pool structure */
// 	threadpool_t *tp = malloc(sizeof(threadpool_t));
// 	if (!tp) {
// 		return NULL;
// 	}
//
// 	/* Initialize */
// 	memset(tp, 0, sizeof(threadpool_t));
// 	tp->threads        = thread_ids;
// 	tp->num_threads    = num_threads;
// 	tp->shutting_down  = false;
// 	tp->jobs_completed = 0;
//
// 	/* Initialize queue */
// 	memset(&tp->queue, 0, sizeof(queue_t));
//
// 	/* Initialize synchronization primitives */
// 	if (pthread_mutex_init(&tp->queue_lock, NULL) != 0) {
// 		free(tp);
// 		return NULL;
// 	}
//
// 	if (pthread_cond_init(&tp->queue_cond, NULL) != 0) {
// 		pthread_mutex_destroy(&tp->queue_lock);
// 		free(tp);
// 		return NULL;
// 	}
//
// 	return tp;
// }
//
// int tp_enqueue(threadpool_t *tp, tp_job_fn func, void *arg) {
// 	if (!tp || !func) {
// 		return TP_ERR_NULL;
// 	}
//
// 	pthread_mutex_lock(&tp->queue_lock);
// 	{
// 		/* Check if shutting down */
// 		if (tp->shutting_down) {
// 			pthread_mutex_unlock(&tp->queue_lock);
// 			return TP_ERR_SHUTDOWN;
// 		}
//
// 		/* Check if queue is full */
// 		if (tp->queue.count >= TP_MAX_QUEUE_SIZE) {
// 			pthread_mutex_unlock(&tp->queue_lock);
// 			return TP_ERR_QUEUE;
// 		}
//
// 		/* Enqueue job */
// 		tp->queue.jobs[tp->queue.head].func = func;
// 		tp->queue.jobs[tp->queue.head].arg  = arg;
// 		tp->queue.head = (tp->queue.head + 1) % TP_MAX_QUEUE_SIZE;
// 		tp->queue.count++;
// 	}
// 	pthread_mutex_unlock(&tp->queue_lock);
//
// 	/* Signal a waiting worker */
// 	pthread_cond_signal(&tp->queue_cond);
//
// 	return TP_OK;
// }
//
// int tp_shutdown(threadpool_t *tp, unsigned timeout_ms) {
// 	if (!tp) {
// 		return -1;
// 	}
//
// 	pthread_mutex_lock(&tp->queue_lock);
// 	{
// 		tp->shutting_down = true;
// 	}
// 	pthread_mutex_unlock(&tp->queue_lock);
//
// 	/* Wake all workers to check shutdown flag */
// 	pthread_cond_broadcast(&tp->queue_cond);
//
// 	/* Wait for workers to exit (or timeout) */
// 	if (timeout_ms > 0) {
// 		struct timespec deadline;
// 		clock_gettime(CLOCK_REALTIME, &deadline);
// 		deadline.tv_sec  += timeout_ms / 1000;
// 		deadline.tv_nsec += (timeout_ms % 1000) * 1000000;
// 		if (deadline.tv_nsec >= 1000000000) {
// 			deadline.tv_sec++;
// 			deadline.tv_nsec -= 1000000000;
// 		}
//
// 		/* Wait for all workers to finish (busy-loop; could be smarter) */
// 		/* This is simplified; real implementation would join threads properly
// 		 */
// 		while (timeout_ms > 0) {
// 			pthread_mutex_lock(&tp->queue_lock);
// 			unsigned completed = tp->jobs_completed;
// 			pthread_mutex_unlock(&tp->queue_lock);
//
// 			usleep(10000); /* 10ms sleep */
// 			timeout_ms -= 10;
// 		}
// 	}
//
// 	/* Return number of jobs completed */
// 	pthread_mutex_lock(&tp->queue_lock);
// 	unsigned completed = tp->jobs_completed;
// 	pthread_mutex_unlock(&tp->queue_lock);
//
// 	return (int)completed;
// }
//
// void tp_destroy(threadpool_t *tp) {
// 	if (!tp) {
// 		return;
// 	}
//
// 	pthread_cond_destroy(&tp->queue_cond);
// 	pthread_mutex_destroy(&tp->queue_lock);
// 	free(tp);
// }
//
// int tp_queue_size(threadpool_t *tp) {
// 	if (!tp) {
// 		return -1;
// 	}
//
// 	pthread_mutex_lock(&tp->queue_lock);
// 	int size = (int)tp->queue.count;
// 	pthread_mutex_unlock(&tp->queue_lock);
//
// 	return size;
// }
