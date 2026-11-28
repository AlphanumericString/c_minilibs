# TODO

general todo for the project
- `x` done (generally rm'ed)
- `~` under way / running but not finished due to smthing else
- `:` paused
- ` ` not started, to do
- `>` current tasks

## Cross-cutting

### Coverage
- [:] multi-threaded tests (needs re-compilation for `#ifdef` branches)
  - [ ] sptr
  - [ ] rb
- [:] da -> oscillation around boundary with `auto_shrink`
- [:] restrict MT tests to specific modules
- [:] add MT tests to coverage mapping

### Tooling / CI
- [:] GitHub CI
  - [:] add cppcheck step
  - [ ] add minimum coverage requirement before main
  - [x] norm
  - [x] restrict run to only needed
- [ ] detect glibc < 2.34 and auto-add `-lrt` for `shm_open` (magic `rb` backend).

## Documentation

- [:] resources
  - [:] add resources for new modules
  - [:] resources on current modules - read for ideas on what to add/do
- [~] polish current module docs
  - [>] ill
  - [ ] rb
  - [ ] tls
  - [ ] tpm
- [:] real-project examples
  - [:] da -> CSV/TSV parser or JSON tokenizer using a growable token/byte array.
  - [:] dsu -> Kruskal MST, connected-component labelling on a grid, or image segmentation.
  - [:] hp -> top-K leaderboard, Dijkstra/A* on a small graph, or a timer/event scheduler.
  - [:] ill -> intrusive free-list allocator, scheduler task list, or an LRU recency chain.
  - [:] lruc -> HTTP response cache (ETag + recency eviction) or a memoized query/result cache.
  - [:] lrutc -> DNS resolver cache or session/token cache with TTL and stale-while-revalidate.
  - [:] rb -> lock-free SPSC producer/consumer telemetry queue or audio sample buffer.
  - [:] sptr -> shared config/asset registry, scene-graph ownership, or an observer/subscriber list.
  - [:] tls -> per-thread scratch arena, error context, RNG state, or request-local allocator.
  - [:] tpm -> parallel map/word-count, HTTP fetch worker pool, or recurring job scheduler.
- [:] marketing
  - [:] da
    - [:] growth factor comparison (1.5x, 2x, fixed increment): realloc count and time.
    - [:] auto-shrink on vs off in an oscillating workload.
  - [:] `make bench` that outputs CSV, plus a gnuplot/matplotlib script.
  - [:] `BENCH.md` with the graphs for all modules; commit the PNGs and link them from the README.
    - [:] graph generation rule: `make bench` -> CSV + gnuplot script.
    - [:] modular graph setup, reusable for any module.
    - [:] graph display: README links to the generated MD file.

## Module backlog

### da
- [:] small-buffer optimization tests/benchmarks, since it changes the allocation paths.

### dsu
- [:] cdsu reserve / up-front growth equivalent (no logs)
- [:] make `rdsu_grow(..., 0)` a successful no-op like `cdsu_grow`
- [:] validate the rewind log chain in `rdsu_validate`

### hp
- [:] faster find
- [:] `map_heap` / re-heapified mapping (current `hp_map` returns a positional `void **` array instead)
- [:] tree-order iterators (pre/in/post) on top of `hpi_iter_level`

### ill
- [:] `ill_unique`/`idl_unique`: return the removed count and/or provide a full (non-consecutive) dedup option.
  - use an htable with load factor .7 so `(list_len / .7) = slot nb`; store ptr + hit count; on a miss check `(slot + 1) % bucket_nb`.
  - heavy for little gains + needs a semi-dedicated hmap impl to keep modules separate; heavy code for low scope.

### lru_cache
- [:] iterator on the timed cache over the TTL min-heap (start/last over heap slots like `hp_iter`, level order, boolean to refresh the TTL clock while walking)
- [:] conversions between `lruc` and `lrutc` (`lruc_convert` like `dsu_convert`), dedicated session

### rb
- [x] magic rb
  - [x] memory mapping magic
  - [x] replace math
- [x] iterator (take example on `da`)
  - [x] `_iter`
  - [x] `_iter_rev`
  - [x] `_iter_range`
  - [x] `_advance_it` / `_recede_it`
- [ ] new functions (parity with the `da` module) [warning: skip in-place array-modifying functions such as filter; we don't want to touch the insertion head nor the output head.]
  - [ ] fold (don't discard the rb)
  - [ ] reduce (don't discard the rb)
  - [ ] others

### sptr
- [ ] package `_Generic` in `#ifdef` blocks and check compile ok for older C versions.

### tls
- [ ] `ftls` -> xmacros for more than 1 struct
- [ ] `vtls` + `ctls` iterator
- [ ] `vtls` -> hmaps

### tpm
- [ ] crash jobs catch
- [ ] spikes
  - [:] dispatch new thread on spikes
  - [:] batch jobs on spikes
- [:] heartbeat system -> every x jobs or x ns/ms, + monitor thread

## NEW MODULES - WHOLE MODULE PAUSED

note: new module paused. finish tpm first.
note: new module paused. finish school project first.

sorted by personal familiarity w subject/ ease of project
- [~] HASHMAPS
 - [x] research - already made a small impl in other project & lruc
 - [ ] branch
 - [ ] minimal header
- [~] XARRAY
 - [x] research - long research
 - [x] branch
 - [ ] minimal header
  - [x] design doc in header
  - [~] read kernel xarray doc & pick functions to add
  - [ ] impl functions
- [~] T_STRING (string slices/non-null terminated strings)
 - [x] research - already implemented in c42lib - re-do in speparated module, usefull
 - [ ] branch
 - [ ] minimal header
- [~] LEAST COMMON PREFIX ARRAYS
 - [~] research - basically a trie where you merge single child branche, mark nodes as yes|no if word ok|not ok
 - [ ] branch
 - [ ] minimal header
- [~] MATRIXES - 3b1b <3 - maths, double as base units
 - [~] research
 - [x] branch
 - [ ] minimal header
- [ ] NEURAL NETS
 - [~] research - 3b1b <3 - maths, applied
 - [x] branch
 - [ ] minimal header
- [~] DISTRIBUTED SPARSE SET
 - [ ] research
 - [x] branch
 - [ ] minimal header
 - note: this is supposed to replace the simple ss implementation we have in a branch
- [ ] INTRUSIVE TREE NODES
 - [ ] research
 - [ ] branch
 - [ ] minimal header
- [ ] CONTROL FLOW INTEGRITY CHECKER
 - [ ] research
 - [ ] branch
 - [ ] minimal header
- [ ] MERKLE TREES
 - [ ] research
 - [ ] branch
 - [ ] minimal header
- [ ] LEXER (GENERALIZED NON-SPECIFIC)
 - [ ] research
 - [ ] branch
 - [ ] minimal header
- [ ] DISJOINT SPARSE TABEL
 - [ ] research
 - [ ] branch
 - [ ] minimal header
 - [ ] src: https://discuss.codechef.com/t/tutorial-disjoint-sparse-table/17404
 - [ ] src: https://cp-algorithms.com/data_structures/sparse-table.html
- [ ] TREAP
 - [ ] research
 - [ ] branch
 - [ ] minimal header

## Resources

### DA : resources

**Reading**
- Rustonomicon, "Implementing Vec": the same problem you solved (growth, alloc failure, ownership), explained well.
- Folly's `FBVector` doc: why a 1.5x growth factor lets freed blocks get reused.
- Drepper, *What Every Programmer Should Know About Memory*: why contiguous arrays win.
- Linux kernel docs on XArray and kfifo, since you're borrowing their style.
- Ryan Fleury, "Enter the Arena": arena allocators.

**Related structures**
- Magic ring buffer.
- Gap buffer (text editors).
- Chunked deque.
- Arena/pool allocator: have your array allocate from one.
- LLVM `SmallVector`: your small-buffer idea, studied in production.
- Open-addressing hash table (Swiss table).
- Bitset or sparse set.

**Talks**
- Mike Acton, "Data-Oriented Design and C++" (CppCon 2014)
- Chandler Carruth, "Tuning C++: Benchmarks, and Compilers, and Compilers! Oh My!" (CppCon 2015), for making your benchmarks credible
- Andrei Alexandrescu, "std::allocator Is to Allocation what std::vector Is to Vexation" (CppCon 2015)
- Matt Kulukundis, "Designing a Fast, Efficient, Cache-friendly Hash Table, Step by Step" (CppCon 2017)

### DSU : resources

**Reading**
- CLRS, chapter on disjoint sets (union by rank, path compression, amortized analysis).
- cp-algorithms, "Disjoint Set Union": compact implementation notes and applications.
- Tarjan, "Efficiency of a Good But Not Linear Set Union Algorithm" (1975): the classic complexity result.
- Tarjan & van Leeuwen, "Worst-case analysis of set union algorithms": path-compression variants.
- Wikipedia: disjoint-set data structure, union by rank, path compression, inverse Ackermann.
- Boost.DisjointSets docs and LLVM/compiler union-find code for production API shape.
- Competitive-programming notes on rollback DSU for the rewindable `rdsu`.

**Related structures**
- Union-find with rollback (persistent/undoable DSU).
- Dynamic connectivity (Holm–de Lichtenberg–Thorup).
- Binary lifting / LCA; Tarjan's offline LCA uses DSU.
- Kruskal MST, connected components, image segmentation, clustering.
- Partition refinement (Hopcroft).
- Disjoint sparse table for static range queries.

**Talks**
- MIT 6.046J / 6.006 lectures on union-find and amortized analysis.

### HP : resources

**Reading**
- CLRS, chapter 6 (heapsort / binary heaps) and chapter 19 (Fibonacci heaps for contrast).
- Wikipedia: binary heap, d-ary heap, heapify.
- Boost.Heap docs: pushpop, replace and mutable-heap operations.
- cppreference `std::priority_queue`: the push/pop interface this module mirrors.
- Linux kernel `include/linux/minheap.h`: compact min-heap macros.

**Related structures**
- Fibonacci, pairing, skew and leftist heaps (mergeable alternatives).
- Binary search trees and indexed skip lists, for order-searchable variants of `hp_find`.
- Priority-queue-based graph algorithms (Dijkstra, A*, Prim) and top-K selection.

### ILL : resources

**Reading**
- Linux kernel `include/linux/list.h` and `hlist`: intrusive doubly linked lists and hash chains.
- BSD `queue(3)` / `sys/queue.h`: SLIST, LIST, STAILQ and TAILQ macro families.
- Boost.Intrusive docs: production intrusive containers and hooks.
- CLRS 10.2 (linked lists) and 11 (chaining in hash tables).
- `container_of` / `offsetof` documentation and examples.
- Wikipedia: linked list, XOR linked list, unrolled linked list, skip list.
- *The Art of Computer Programming*, vol. 1, section 2.2.5 (linked lists).
- Lock-free linked-list papers (Harris; Michael) for the concurrency angle.

**Related structures**
- Singly/doubly linked lists with sentinels vs NULL-terminated lists.
- Circular lists and list descriptors/handlers.
- XOR linked list (memory-efficient doubly linked).
- Unrolled linked list.
- Skip list (layered linked lists).
- Intrusive trees (RB/AVL) and `hlist`.
- Lock-free linked lists and hazard pointers.

### LRU_CACHE (lruc / lrutc) : resources

**Reading**
- CLRS, chapter 11 (hash tables) and 10.2 (linked lists): the two indexes the cache bolts together over shared nodes.
- Belady's optimal MIN algorithm: the reference bound LRU is compared against.
- Wikipedia: cache replacement policies (LRU, LFU, ARC, CLOCK/second-chance, 2Q) and cache stampede.
- CLRS chapter 6 (heaps) and the `hp` module docs, since the timed cache keeps a min-heap of expiry deadlines.
- Rust `lru` crate and Python `functools.lru_cache` docs: the API surface users expect, including the on-miss generator.
- nginx/Varnish cache docs on TTL, stale-while-revalidate and negative caching.
- `redis`/`memcached` internals: production eviction and expiry designs.

**Related structures**
- LFU, ARC, segmented LRU (2Q), CLOCK/second-chance: approximate or adaptive alternatives to LRU.
- Open-addressing hash map (Swiss table) as a faster lookup index than chaining.
- Timing wheels (hierarchical timers) as an O(1) alternative to the TTL min-heap.
- Bloom filters for negative caching, consistent hashing for distributed caches.
- Slab/pool allocators for node storage (the reserved nodes list is a small step in that direction).

### RB : resources

**Reading**
- Wikipedia: circular buffer, ring buffer, producer–consumer problem.
- Linux kernel `kfifo` (`include/linux/kfifo.h`, `lib/kfifo.c`): compact ring-buffer API and power-of-two indexing.
- Boost.CircularBuffer docs: iterator and capacity semantics.
- Lamport, "Proving the Correctness of Multiprocess Programs" (1977): the original SPSC reasoning.
- Preshing, "Atomic Weapons" and memory-ordering articles.
- cppreference: `std::atomic`, `memory_order_acquire`/`release`, `stdatomic.h`.
- Dmitry Vyukov's bounded MPMC queue and intrusive MPSC queue articles.
- LMAX Disruptor technical paper: ring buffer as a low-latency queue.
- "Magic ring buffer" / `mmap`-based double-mapping trick (Linux `vmsplice` discussions).

**Related structures**
- Double/triple buffering.
- Chunked queue / linked-chunk ring.
- Lock-free SPSC/MPMC queues (Vyukov, Michael–Scott).
- LMAX Disruptor.
- Gap buffer (text editors).
- Pipeline / streaming buffers and `io_uring` submission/completion rings.

**Talks**
- Martin Thompson, "LMAX Disruptor" (InfoQ), on ring-buffer design and mechanical sympathy.
- Herb Sutter, "atomic<> Weapons" (C++ and Beyond 2012), on atomics and memory order.
- Dmitry Vyukov, talks on lock-free queues and bounded MPMC queues.

### SPTR : resources

**Reading**
- cppreference: `std::unique_ptr`, `std::shared_ptr`, `std::weak_ptr`, `std::make_shared`, `enable_shared_from_this`.
- Boost.SmartPtr docs: `intrusive_ptr`, `shared_ptr`, `weak_ptr` and ownership rules.
- Rust `Rc`, `Arc` and `Weak` docs: the same model with different ergonomics.
- *The Rustonomicon*, ownership and `Drop` chapters.
- Wikipedia: reference counting, weak reference, cycle detection, dangling pointer.
- libstdc++/libc++ `shared_ptr` control-block implementations: one allocation, atomic counts.
- Hans Boehm, "Threads Cannot Be Implemented as a Library" (2004), for the memory-model angle.
- C++ standard atomic memory-order rules for refcount acquire/release.

**Related structures**
- Garbage collection: mark-sweep, reference counting, cycle collection.
- Intrusive reference counting (`boost::intrusive_ptr`).
- Hazard pointers and epoch-based reclamation.
- Rust `Rc`/`Arc`, C++ `shared_ptr` control block.
- Arena/pool allocators and ownership handles.
- Copy-on-write and immutable persistent structures.

**Talks**
- Herb Sutter, "Leak-Freedom in C++... By Default" (CppCon 2016), on ownership and smart pointers.

### TLS : resources

**Reading**
- POSIX `pthread_key_create(3)`, `pthread_getspecific(3)`, `pthread_setspecific(3)` man pages.
- Ulrich Drepper, "ELF Handling For Thread-Local Storage": `__thread`/`_Thread_local` models and relocation types.
- GCC and Clang docs on `__thread` / `thread_local` storage classes.
- cppreference: `thread_local`, `std::this_thread`, `std::thread`.
- Wikipedia: thread-local storage, `errno`, reentrancy.
- glibc NPTL TLS implementation notes and `pthread_cleanup_push`/`pop` destructor semantics.
- *Programming with POSIX Threads* (Butenhof), TLS and thread-specific data chapters.

**Related structures**
- Per-thread arenas/allocators and caches.
- C++ `thread_local`, Rust `thread_local!`.
- TLS destructors and cleanup handlers.
- Coroutine-local / fiber-local storage.
- Context/error objects such as `errno` and `GetLastError`.
- OpenMP `threadprivate`; hash maps for keyed TLS (`vtls` upgrade).

### TPM : resources

**Reading**
- Butenhof, *Programming with POSIX Threads*: mutexes, condition variables, thread lifecycle.
- Williams, *C++ Concurrency in Action*: thread pools, futures, work stealing.
- Herlihy & Shavit, *The Art of Multiprocessor Programming*: queues, work stealing, progress guarantees.
- Blumofe & Leiserson, "Scheduling Multithreaded Computations by Work Stealing" (1999).
- Wikipedia: thread pool, work stealing, producer–consumer, futex.
- Drepper, "Futexes Are Tricky": fast userspace locking and wake-up paths.
- cppreference: `std::thread`, `std::async`, `std::packaged_task`, `std::condition_variable`.
- Cilk, TBB and Rust Rayon internals: production work-stealing schedulers.
- Dmitry Vyukov, "Scalable Go Scheduler Design": M:N scheduling and run queues.
- Linux `io_uring` worker pools and libuv thread pool: real-world designs.

**Related structures**
- Work-stealing deques (Chase–Lev).
- Lock-free MPMC queues (Michael–Scott, Vyukov).
- Fork-join pools (Cilk, TBB, Rayon).
- Event loops / reactors as an alternative to thread pools.
- Actor model and message queues.
- Coroutines, fibers and green threads.
- Timing wheels for recurring/heartbeat jobs.

**Talks**
- Dmitry Vyukov, "Go scheduler: Implementing language with lightweight concurrency" (GopherCon 2014).
- Herb Sutter, "The Free Lunch Is Over" (article and talks).
- Martin Thompson, "Mechanical Sympathy" talks on queues and latency.
