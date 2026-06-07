# Minimal C Thread Pool Library Design

## Core Philosophy
**Lightweight, predictable, and boring** — do one thing well. Avoid feature creep. Users manage complexity, library manages concurrency safely.

---

## SHOULD HANDLE

### Core Responsibilities
1. **Thread lifecycle management**
   - Creation, pooling, and graceful shutdown
   - Thread reuse to amortize creation costs
   
2. **Work queue management**
   - FIFO job dispatch (simplest, most predictable)
   - Bounded or unbounded queue options
   - Thread-safe enqueue/dequeue

3. **Synchronization primitives**
   - Mutex + condition variable coordination
   - Blocking on queue empty/full states

4. **Basic error handling**
   - Return error codes for invalid states
   - Resource exhaustion detection

5. **Graceful shutdown**
   - Wait for pending jobs or timeout
   - Clean thread termination

---

## SHOULD NOT HANDLE

### Out of Scope
1. **Job priorities** — Use external wrapper/priority queue if needed
2. **Job cancellation** — Job is either queued or executing; user manages
3. **Job dependencies** — User orchestrates via callbacks or higher-level lib
4. **Load balancing** — Simple FIFO is sufficient; user can wrap
5. **Work stealing** — Adds complexity; FIFO is predictable
6. **Memory pooling** — malloc/free is fine; user brings their own allocator if needed
7. **Metrics/monitoring** — No counters or stats exposed (keep it simple)
8. **Pinning/affinity** — User calls pthread_setaffinity_np if needed
9. **Backpressure handling** — User decides: block, reject, or use timeout
10. **Job result storage** — User manages via job context/callbacks

---

## OWNERSHIP & LIFECYCLE

### Library TAKES OWNERSHIP
- **Thread handles** — Created, managed, destroyed by library
- **Condition variables & mutexes** — Hidden internal synchronization
- **Work queue structure** — Internal linked list or ring buffer
- **Thread state machine** — Idle → executing → idle → shutdown

### USER TAKES OWNERSHIP
- **Job struct definition** — User defines what a job is (flexible)
- **Job cleanup/resources** — If job allocates memory, user frees (in callback)
- **Memory allocations** — malloc/free called by user or provided allocator
- **Thread-local state** — User manages if needed
- **Job scheduling policy** — User decides when/how to enqueue

### Shared Responsibility
- **Shutdown semantics** — Library provides `tp_shutdown()`, user calls it (must be external)
- **Error handling** — Library reports, user decides action

---

## PUBLIC API EXPOSURE

### What to Expose (Minimal)
```c
// Opaque handle
typedef struct threadpool threadpool_t;

// Job callback signature
typedef void (*tp_job_fn)(void *arg);

// Core operations
threadpool_t* tp_create_ex(pthread_t *thread_ids, unsigned num_threads);
int tp_enqueue(threadpool_t *tp, tp_job_fn func, void *arg);
int tp_shutdown(threadpool_t *tp, unsigned timeout_ms);
void tp_destroy(threadpool_t *tp);

// Status
int tp_queue_size(threadpool_t *tp);
```

### What to Hide
- Mutex/condition variable details
- Thread data structures
- Queue implementation details
- Internal thread state
- Wake-up/signaling internals

---

## DESIGN DECISIONS & RATIONALE

### 1. Queue Strategy: Bounded with #define
**FINAL: Bounded queue, user overrides via #define**
```c
// In threadpool.h or config
#define TP_MAX_QUEUE_SIZE 1024

// User can override:
// gcc -DTP_MAX_QUEUE_SIZE=4096 ...
```
**Rationale:**
- Simple, predictable memory usage
- User controls limit without recompile (via -D flag)
- Enqueue fails cleanly when full (TP_ERR_QUEUE)
- No malloc/free overhead for queue entries

### 2. Thread Creation: Pre-created Thread IDs
**FINAL: User provides pthread_t array, library takes ownership**
```c
threadpool_t* tp_create_ex(pthread_t *thread_ids, unsigned num_threads);
```
**Rationale:**
- User owns thread creation and attribute setup
- User calls `pthread_create()` with custom attributes (stack size, affinity, etc.)
- Library only manages the pool lifecycle and queue
- No hidden pthread_create calls; full transparency
- Easier to test and debug thread setup

**Flow:**
```c
// User setup
pthread_t threads[4];
for (int i = 0; i < 4; i++) {
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    // Set custom attributes
    pthread_attr_setstacksize(&attr, 64*1024);
    pthread_create(&threads[i], &attr, tp_worker_func, NULL);
    pthread_attr_destroy(&attr);
}
// Pass to library
threadpool_t *tp = tp_create_ex(threads, 4);
```

### 3. Shutdown Semantics: Timeout in Milliseconds
**FINAL: `tp_shutdown(tp, timeout_ms)`**
```c
int tp_shutdown(threadpool_t *tp, unsigned timeout_ms);
```
- Returns: number of jobs completed before timeout
- Returns `-1` on error
- Semantic: stop accepting new jobs, wait up to timeout_ms for completion
- Edge case: `timeout_ms = 0` means "don't wait, kill workers"

### 4. Thread Safety Guarantees
**FINAL: Pool object is thread-safe, jobs are NOT**
- Multiple threads can safely `tp_enqueue()` concurrently
- Job execution is serial per worker (no shared state between jobs on same thread)
- User is responsible for synchronizing within jobs
- **Important:** Worker threads execute jobs you provide; pool doesn't create them

### 5. Error Handling: Integer Return Codes
**FINAL: Simple return codes**
```c
#define TP_OK             0
#define TP_ERR_NULL       1  // NULL pointer
#define TP_ERR_QUEUE      2  // Queue full (bounded)
#define TP_ERR_SHUTDOWN   3  // Pool shutting down
#define TP_ERR_ALLOC      4  // Memory allocation failed (pool struct)
```

### 6. Initialization: Pre-created Threads Only
**FINAL: `tp_create_ex(thread_ids, count)` — NO default variant**
- Library does NOT call `pthread_create()`
- User has 100% control over thread setup
- Pool struct allocated via malloc (or could add allocator later)
- Simpler, less hidden magic

**Rationale:** User creating threads is not that hard, and gives them full control.

---

## FILE STRUCTURE (< 20 files)

```
threadpool/
├── threadpool.h          # Public API
├── threadpool.c          # Main implementation
├── queue.h               # Internal work queue
├── queue.c               # Queue implementation
├── atomic.h              # Atomic ops helpers (wrapper)
├── example.c             # Simple example
├── example_advanced.c    # Advanced usage
├── test_basic.c          # Unit tests
├── test_stress.c         # Stress/concurrency tests
└── Makefile
```

**Total: ~10 files** ✓

---

## EXAMPLE API USAGE

```c
#include <pthread.h>
#include "threadpool.h"

// Define work
void process_item(void *arg) {
    int *id = (int *)arg;
    printf("Processing item %d\n", *id);
    free(id);
}

int main(void) {
    // Step 1: Create threads with custom attributes
    pthread_t threads[4];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 64 * 1024);  // 64KB stack
    
    for (int i = 0; i < 4; i++) {
        pthread_create(&threads[i], &attr, tp_worker, NULL);
    }
    pthread_attr_destroy(&attr);
    
    // Step 2: Create pool with pre-created threads
    threadpool_t *tp = tp_create_ex(threads, 4);
    if (!tp) {
        fprintf(stderr, "Failed to create threadpool\n");
        return 1;
    }
    
    // Step 3: Submit work
    for (int i = 0; i < 100; i++) {
        int *id = malloc(sizeof(int));
        *id = i;
        int ret = tp_enqueue(tp, process_item, id);
        if (ret != TP_OK) {
            fprintf(stderr, "Enqueue failed: %d\n", ret);
            free(id);
        }
    }
    
    // Step 4: Shutdown with 5-second timeout
    unsigned completed = tp_shutdown(tp, 5000);
    printf("Jobs completed before timeout: %u\n", completed);
    
    // Step 5: Cleanup
    tp_destroy(tp);
    
    return 0;
}
```

---

## DECISIONS TO MAKE

1. **Bounded queue?** (adds complexity vs simplicity)
2. **Per-thread context/TLS?** (not exposed; user manages in job)
3. **Job timeouts?** (out of scope; user wraps if needed)
4. **Statistics API?** (omit for minimalism)
5. **Pause/resume?** (omit; too complex)
6. **Pinning to cores?** (out of scope)
7. **Custom allocator?** (yes, as optional `_ex()` variant)
8. **Atomic ops fallback?** (use `<stdatomic.h>` if available, else fallback)

---

## Trade-offs Summary

| Feature | Included? | Rationale |
|---------|-----------|-----------|
| Job scheduling | ✓ | Core feature |
| Thread pooling | ✓ | Core feature |
| Graceful shutdown | ✓ | Essential |
| Job priorities | ✗ | Adds complexity; user wraps |
| Job cancellation | ✗ | Hard to implement safely |
| Metrics | ✗ | Omit for minimalism |
| Backpressure | ✗ | User decides policy |
| Custom allocator | ✓ (optional) | Useful for embedded |
| Thread affinity | ✗ | Out of scope |
| Work stealing | ✗ | FIFO sufficient |

---

## DECISIONS FINALIZED ✓

1. **Bounded queue** → #define TP_MAX_QUEUE_SIZE, user overrides via -D
2. **Thread creation** → User provides pre-created pthread_t array
3. **Shutdown** → timeout in milliseconds, returns jobs completed
4. **Custom allocator** → No; pool allocates itself via malloc (kept simple)
5. **Queue size visibility** → tp_queue_size() exposed
6. **Thread safety** → Pool is thread-safe; jobs are not
7. **Error handling** → Integer return codes

## REMAINING DESIGN DETAILS

1. **Worker thread function signature** — Internal, called by pthread_create
   - Must accept `void *arg` (pool pointer)
   - Signals readiness to main thread
   - Loops: wait for job → execute → loop

2. **Graceful shutdown mechanism**
   - Set "shutting down" flag
   - Stop accepting new jobs (return TP_ERR_SHUTDOWN)
   - Let worker threads drain queue
   - After timeout_ms, wake all and exit
   - Remaining jobs in queue are abandoned (user's job to cleanup if needed)

3. **Queue representation**
   - Ring buffer or linked list?
   - **Proposal:** Bounded ring buffer (fixed size, simpler)
   - Head/tail pointers with count

4. **Worker thread synchronization**
   - One mutex + one condition variable
   - Workers wait on: queue not empty OR shutdown flag
   - Enqueuer signals after adding job
   - Shutdown signals all waiting threads

5. **Memory cleanup on error**
   - tp_create_ex returns NULL if pool struct allocation fails
   - No leaks; if malloc fails, tp_create_ex cleans up and returns NULL
   - User still owns threads; must pthread_join or pthread_cancel
