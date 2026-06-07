# Minimal C Thread Pool Library - Final Design Summary

## Design Principles

1. **Do one thing well** — Manage threads and dispatch jobs safely
2. **Minimal surface area** — Only what you absolutely need
3. **User control** — User sets up threads; library doesn't create them
4. **Predictable behavior** — No surprises, bounded queues, timeouts
5. **Boring is good** — Simple FIFO, no fancy scheduling

---

## Core Decisions

### Queue Management: Bounded Ring Buffer
- **Fixed size:** Compile-time configurable via `#define TP_MAX_QUEUE_SIZE`
- **Override:** User can `-DTP_MAX_QUEUE_SIZE=4096` at compile time
- **Behavior:** Enqueue fails with `TP_ERR_QUEUE` when full
- **Why:** Predictable memory, no malloc overhead, prevents memory exhaustion

### Thread Lifecycle: Pre-Created Threads
- **User creates:** `pthread_create()` with custom attributes
- **Library receives:** `pthread_t *thread_ids` array
- **Library owns:** Thread pointers until `tp_destroy()`
- **Why:** Full control over stack size, affinity, scheduling, debug names

```c
// User setup
pthread_t threads[4];
pthread_attr_t attr;
pthread_attr_init(&attr);
pthread_attr_setstacksize(&attr, 64 * 1024);

for (int i = 0; i < 4; i++) {
    pthread_create(&threads[i], &attr, tp_worker, NULL);
}

// Hand to library
threadpool_t *tp = tp_create_ex(threads, 4);
```

### Shutdown: Timeout in Milliseconds
- **Returns:** Number of completed jobs (pre-timeout)
- **Behavior:**
  1. Stop accepting new jobs (return `TP_ERR_SHUTDOWN`)
  2. Let workers drain queue for up to `timeout_ms`
  3. After timeout, wake workers and let them exit
  4. Jobs remaining in queue are abandoned
- **User responsibility:** Clean up any abandoned jobs if needed

### Error Handling: Simple Return Codes
```c
#define TP_OK          0   /* Success */
#define TP_ERR_NULL    1   /* NULL pointer */
#define TP_ERR_QUEUE   2   /* Queue full */
#define TP_ERR_SHUTDOWN 3  /* Pool shutting down */
#define TP_ERR_ALLOC   4   /* Malloc failed */
```

### Job Representation: Function Pointer + Context
- **Signature:** `typedef void (*tp_job_fn)(void *arg);`
- **Flexibility:** User embeds any data structure in `arg`
- **Memory:** User owns all allocations

```c
// Option 1: Pass simple data
tp_enqueue(tp, process_int, (void *)(intptr_t)42);

// Option 2: Pass pointer to struct
work_item_t *item = malloc(sizeof(work_item_t));
item->id = 42;
item->value = 3.14;
tp_enqueue(tp, process_item, item);  // Job frees in callback
```

---

## API Surface

```c
/* Create pool with pre-created threads */
threadpool_t* tp_create_ex(pthread_t *thread_ids, unsigned num_threads);

/* Submit job */
int tp_enqueue(threadpool_t *tp, tp_job_fn func, void *arg);

/* Graceful shutdown with timeout (milliseconds) */
int tp_shutdown(threadpool_t *tp, unsigned timeout_ms);

/* Cleanup */
void tp_destroy(threadpool_t *tp);

/* Monitor queue depth */
int tp_queue_size(threadpool_t *tp);

/* Internal: worker thread function (user passes to pthread_create) */
void* tp_worker(void *arg);
```

---

## Thread Safety

### Pool is Thread-Safe
- `tp_enqueue()` can be called from multiple threads concurrently
- Mutex protects queue and state
- Condition variable coordinates workers

### Jobs are NOT Thread-Safe
- Each worker executes jobs serially
- No job runs simultaneously with another on the same thread
- Job body is user's responsibility to synchronize (if needed)

---

## Memory Ownership

| Resource | Allocator | Deallocator | Notes |
|----------|-----------|-------------|-------|
| Pool struct | malloc (internal) | free (tp_destroy) | Library manages |
| Threads | User | User | User creates, library uses |
| Queue entries | Ring buffer (fixed) | N/A | No mallocs in queue |
| Job data | User | User | Pass via arg pointer |
| Mutex/cond | pthread_*_init | pthread_*_destroy | Library manages |

---

## Implementation Details

### Worker Thread Main Loop
```
LOOP:
  1. Lock queue
  2. Wait for (job in queue) OR (shutdown flag)
  3. If job exists: dequeue and mark complete count
  4. If shutdown AND queue empty: unlock and exit
  5. Unlock queue
  6. If job: execute outside lock
  7. GOTO LOOP
```

### Synchronization
- **One mutex** protects queue and shutdown flag
- **One condition variable** for job availability and shutdown signal
- **Broadcast** on shutdown to wake all workers
- **Signal** on enqueue to wake one worker

### Graceful Shutdown
1. Set `shutting_down = true`
2. Broadcast on condition variable
3. Wait up to `timeout_ms` (simplified: polling with usleep)
4. Workers check flag and exit when queue is empty

---

## What's NOT Included (And Why)

| Feature | Reason |
|---------|--------|
| Job priorities | Complexity vs frequency tradeoff; user wraps if needed |
| Job cancellation | Hard to implement safely; jobs are execute-once |
| Work stealing | FIFO is predictable; complexity not worth it |
| Job timeouts | User provides timeout wrapper around job |
| Metrics/stats | Library stays minimal; user instruments if needed |
| Thread affinity | User calls pthread_setaffinity_np before handoff |
| Priority scheduling | Out of scope; use nice() or realtime if needed |
| Backpressure callbacks | Simple: enqueue returns error if full |
| Result storage | User passes result pointer in job context |

---

## File Count Target: ✓ Under 20

```
threadpool/
├── threadpool.h          # Public API + docs
├── threadpool.c          # Implementation
├── example.c             # Usage examples
├── example_advanced.c    # Advanced patterns (optional)
├── test_unit.c           # Basic unit tests
├── test_stress.c         # Concurrency stress tests
└── Makefile
```

**Total: 7 core files** (well under 20)

---

## Design Trade-offs

### Bounded Queue vs Unbounded
- **Chosen:** Bounded
- **Pro:** Predictable, prevents memory exhaustion, forces user to think about capacity
- **Con:** Must handle TP_ERR_QUEUE; slightly more complex API

### Pre-created Threads vs Library-Created
- **Chosen:** Pre-created
- **Pro:** User controls attributes, testability, simplicity
- **Con:** Slightly more boilerplate for user

### Timeout vs Blocking Forever
- **Chosen:** Timeout
- **Pro:** Prevents hangs, allows graceful degradation
- **Con:** User must check return value

### FIFO vs Priority Queue
- **Chosen:** FIFO
- **Pro:** Simpler, more predictable, fairer
- **Con:** No job prioritization; user wraps if needed

---

## Common Usage Patterns

### Pattern 1: Fire and Forget
```c
threadpool_t *tp = tp_create_ex(threads, 4);
for (int i = 0; i < 1000; i++) {
    tp_enqueue(tp, work_func, (void *)(intptr_t)i);
}
tp_shutdown(tp, 30000);  /* 30 second timeout */
tp_destroy(tp);
```

### Pattern 2: Structured Work
```c
typedef struct {
    char *filename;
    int priority;
} task_t;

void process_file(void *arg) {
    task_t *t = (task_t *)arg;
    /* ... */
    free(t->filename);
    free(t);
}

// Enqueue
for (int i = 0; i < N; i++) {
    task_t *t = malloc(sizeof(task_t));
    t->filename = strdup(filenames[i]);
    tp_enqueue(tp, process_file, t);
}
```

### Pattern 3: Backpressure Control
```c
while (1) {
    int ret = tp_enqueue(tp, job_func, context);
    if (ret == TP_ERR_QUEUE) {
        printf("Queue full, backing off\n");
        usleep(100000);  /* Back off 100ms */
        continue;
    }
    /* Success or fatal error */
    break;
}
```

### Pattern 4: Monitoring
```c
while (tp_queue_size(tp) > 0) {
    printf("Queue depth: %d\n", tp_queue_size(tp));
    usleep(100000);
}
```

---

## Next Steps for Implementation

1. **Implement tp_worker()** — Main loop with proper synchronization
2. **Test error paths** — NULL checks, queue full, shutdown during enqueue
3. **Test shutdown behavior** — Verify jobs complete before timeout
4. **Stress test** — High job throughput, rapid shutdown, thread contention
5. **Measure memory** — Verify no leaks, bounded allocation
6. **Document examples** — Pattern cookbook for users

---

## Key Design Insight

> **User creates threads, library manages dispatch.**
> 
> This clean separation ensures:
> - User has full control over thread setup
> - Library can't make bad decisions about attributes
> - Testing is easier (can mock threads)
> - Deployment flexibility (adjust stack size, affinity, etc.)

This is why we chose pre-created threads over `tp_create(num_threads)`.
