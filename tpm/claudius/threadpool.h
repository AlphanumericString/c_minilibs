#ifndef THREADPOOL_H
#define THREADPOOL_H

// #include <pthread.h>
// #include <stdbool.h>
//
// /* ============================================================================
//  * Minimal C Thread Pool Library
//  *
//  * Features:
//  * - User-managed thread creation (full control over pthread attributes)
//  * - Bounded FIFO work queue (configurable via TP_MAX_QUEUE_SIZE)
//  * - Graceful shutdown with timeout
//  * - Thread-safe enqueue
//  * - No job result storage (user passes context via void *arg)
//  *
//  * Non-features (intentionally omitted):
//  * - Job priorities
//  * - Job cancellation
//  * - Work stealing / load balancing
//  * - Metrics/monitoring
//  * - Custom allocators
//  * ========================================================================== */
//
// /* Maximum queue size (override with -DTP_MAX_QUEUE_SIZE=N) */
// #ifndef TP_MAX_QUEUE_SIZE
// #	define TP_MAX_QUEUE_SIZE 1024
// #endif
//
// /* Error codes */
// #define TP_OK           0 /* Operation succeeded */
// #define TP_ERR_NULL     1 /* NULL pointer argument */
// #define TP_ERR_QUEUE    2 /* Queue is full (bounded queue) */
// #define TP_ERR_SHUTDOWN 3 /* Pool is shutting down */
// #define TP_ERR_ALLOC    4 /* Memory allocation failed */
//
// /* Job callback signature */
// typedef void (*tp_job_fn)(void *arg);
//
// /* Job queue entry */
// typedef struct {
// 	tp_job_fn func;
// 	void     *arg;
// } job_t;
//
// /* Bounded ring buffer for job queue */
// typedef struct {
// 	job_t    jobs[TP_MAX_QUEUE_SIZE];
// 	unsigned head;  /* Next write position */
// 	unsigned tail;  /* Next read position */
// 	unsigned count; /* Number of jobs in queue */
// } queue_t;
//
// /* Thread pool handle (opaque to users) */
// typedef struct threadpool {
// 	pthread_t *threads;
// 	unsigned   num_threads;
//
// 	queue_t         queue;
// 	pthread_mutex_t queue_lock;
// 	pthread_cond_t  queue_cond;
//
// 	bool     shutting_down;
// 	unsigned jobs_completed; /* Track for return value of tp_shutdown */
// } threadpool_t;
// /* ============================================================================
//  * Public API
//  * ========================================================================== */
//
// /**
//  * tp_create_ex - Create a thread pool with pre-created threads
//  *
//  * @thread_ids: Array of pthread_t handles, already created by caller
//  * @num_threads: Number of threads in the array
//  *
//  * Return: Pool handle on success, NULL on failure (TP_ERR_ALLOC)
//  *
//  * NOTE: User owns the threads and must manage their lifecycle.
//  *       Pool takes ownership of the array contents and will use them
//  *       until tp_destroy() is called.
//  *
//  * Example:
//  *   pthread_t threads[4];
//  *   pthread_attr_t attr;
//  *   pthread_attr_init(&attr);
//  *   pthread_attr_setstacksize(&attr, 64 * 1024);
//  *   for (int i = 0; i < 4; i++) {
//  *       pthread_create(&threads[i], &attr, tp_worker, NULL);
//  *   }
//  *   threadpool_t *tp = tp_create_ex(threads, 4);
//  */
// threadpool_t *tp_create_ex(pthread_t *thread_ids, unsigned num_threads);
//
// /**
//  * tp_enqueue - Submit a job to the pool
//  *
//  * @tp: Pool handle
//  * @func: Job function to execute
//  * @arg: Context pointer passed to func
//  *
//  * Return: TP_OK on success
//  *         TP_ERR_NULL if tp or func is NULL
//  *         TP_ERR_QUEUE if queue is full (bounded)
//  *         TP_ERR_SHUTDOWN if pool is shutting down
//  *
//  * NOTE: Thread-safe; can be called from multiple threads.
//  *       Job is copied into queue (func + arg), so caller retains ownership
//  *       of any data structures (though arg often points to heap-allocated
//  * data).
//  */
// int tp_enqueue(threadpool_t *tp, tp_job_fn func, void *arg);
//
// /**
//  * tp_shutdown - Gracefully shut down the pool
//  *
//  * @tp: Pool handle
//  * @timeout_ms: Milliseconds to wait for jobs to complete
//  *              0 = don't wait, kill workers immediately
//  *              >0 = wait up to this many milliseconds
//  *
//  * Return: Number of jobs completed before timeout on success
//  *         -1 on error (NULL tp)
//  *
//  * Behavior:
//  * - Stops accepting new jobs (subsequent enqueue calls fail with
//  * TP_ERR_SHUTDOWN)
//  * - Waits up to timeout_ms for worker threads to drain the queue
//  * - After timeout, workers are signaled to exit
//  * - Jobs remaining in queue are abandoned (caller must clean up if needed)
//  *
//  * NOTE: Must be called by external code; not automatic.
//  *       This allows caller to decide when shutdown happens.
//  *       After calling tp_shutdown(), do NOT call tp_enqueue().
//  */
// int tp_shutdown(threadpool_t *tp, unsigned timeout_ms);
//
// /**
//  * tp_destroy - Destroy pool and free resources
//  *
//  * @tp: Pool handle (can be NULL; no-op if so)
//  *
//  * Return: void
//  *
//  * NOTE: Must be called AFTER tp_shutdown().
//  *       Does NOT join threads; assumes workers have exited or been cancelled.
//  *       Frees the pool struct and internal queue.
//  */
// void tp_destroy(threadpool_t *tp);
//
// /**
//  * tp_queue_size - Get current number of jobs in queue
//  *
//  * @tp: Pool handle
//  *
//  * Return: Number of queued (not yet executing) jobs
//  *         -1 on error (NULL tp)
//  *
//  * NOTE: Useful for algorithms that need to know queue depth.
//  *       Value is a snapshot; may change immediately after return.
//  */
// int tp_queue_size(threadpool_t *tp);
//
// /* ============================================================================
//  * Worker Thread Function (internal, but user provides to pthread_create)
//  *
//  * User must pass tp_worker as the thread function when creating threads:
//  *   pthread_create(&threads[i], &attr, tp_worker, pool_ptr);
//  *
//  * The pool_ptr argument is NOT the pool handle; it's an internal detail
//  * passed by the pool initialization.
//  * ========================================================================== */
//
// void *tp_worker(void *arg);
//
#endif /* THREADPOOL_H */
