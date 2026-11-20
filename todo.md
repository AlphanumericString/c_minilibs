# TODO

general todo for the project
- `x` done (generally rm'ed)
- `~` under way / running but not finished due to smthing else
- `:` paused
- ` ` not started, to do
- `>` current tasks

## General TODO:
- [>] refactor
 - [>] ill-> api consistency around head
 - [ ] rptr->package _generic in ifdef blocks and check compile ok for c older version
 - [ ] tls->vtls->hmaps
 - [ ] tls->vtls->hmaps.
- [~] coverage
 - [:] multi threaded tests (needs re-comp for #ifdef branches)
  - [ ] sptr
  - [ ] rb
 - [:] da
  - [ ] oscilation around boudary with auto_shrink
 - [x] keep at 100% for branch, lines, region, funcs
- [:] example
 - [:] rb->example w producer/consumer setup
- [>] impl
 - [~] tooling
  - [~] github CICD
   - [:] add cppcheck step
   - [ ] add minimum coverage requireemnt before main
   - [ ] norm
   - [ ] restrict run to only needed
 - [>] code
  - [:] new modules (see bellow)
  - [~] old module functionalities (detail bellow)
   - [x] hp-> more funcs, iterators
   - [x] lruc->make a separate lru only cache similar to dsu/ (lruc pure + lrutc timed)
   - [>] ill->circular lists, elem centric funcs (dont take node, but elem containing the ill/idl ptr), iteraors
   - [ ] rb->add functs to module, iterators
   - [ ] tls->ftls->xmacros for more than 1 struct, vtls+ctls iterator
   - [ ] tpm->crash catch, job batching on stress conditions, atomic thrd nb - increase/remove thrd control handle, auto increase thrd nb on stress
 - [:] coverage
  - [:] add multi threaded tests
   - [:] restrict to specific modules
   - [:] add multi thread tests to coverage mapping
- [:] doc
 - [:] ressources
  - [:] add ressources for todo new modules
  - [:] ressources on curr modules - read for ideas on what to add/do
 - [~] polish
  - [~] finish todo for current modules
   - [~] check section and execute
    - [x] hp
    - [x] lruc
    - [ ] ill
    - [ ] rb
    - [ ] tls
    - [ ] tpm
  - [:] da
   - [:] A real example, such as a tokenizer or CSV parser built on the library.
   - [:] Small-buffer optimization tests/benmarks since it changes the allocation paths.
  - [:] dsu
   - [:] A real example.
  - [:] rb
   - [:] A real example.
    - [:] rb->example w producer/consumer setup
  - [:] hp
   - [:] A real example.
  - [:] ill
   - [:] A real example.
  - [:] lruc
   - [:] A real example.
  - [:] sptr
   - [:] A real example.
  - [:] tls
   - [:] A real example.
  - [:] tpm
   - [:] A real example.
 - [:] marketing
  - [:] da
   - [:] Growth factor comparison (1.5x, 2x, fixed increment): realloc count and time.
   - [:] Auto-shrink on vs off in an oscillating workload.
  - [:] `make bench` that outputs CSV, plus a gnuplot/matplotlib script. Commit the PNGs to a BENCH.md docuement with all the graphs for all modules
  - [:] Md file with graphs or visible benchmarks.
   - [:] graphs
    - [:] graph gen rule `make bench` -> csv + gnuplot script
    - [:] graph setup -> modular for any module
    - [:] graph display -> readme links to the newly md file created

### IMPLEMENTATION
- [x] hp
 - [x] more funcs (eg map, filter, ...) take example on da
 - [x] iterator (take example on da)
 - [:] faster find
 - [:] map_heap / re-heapified mapping (current hp_map returns a positional void ** array instead)
 - [:] tree-order iterators (pre/in/post) on top of hpi_iter_level
 - [ ] reduce_to: merge into a caller-provided element, no allocation
- [ ] da
 - [ ] reduce_to: merge into a caller-provided element, no allocation
- [~] lru
 - [x] split pure lru from the ttl+lru current setup and exec in a split setup like the rdsu/cdsu setup (lruc pure, lrutc timed)
 - [x] iterator on the pure lru (pull iterator over the recency list, forward/reverse, promote_to_mru boolean)
 - [:] iterator on the timed cache over the ttl min heap (start/last over heap slots like hp_iter, level order, boolean to refresh the ttl clock while walking)
 - [:] conversions between lruc and lrutc (lru_cache/lruc_convert/ like dsu_convert/), dedicated session
 - [x] dtor is called on the replaced value when an existing key is updated (both caches)
- [:] ill
 - [>] elem based functions
 - [>] set elem based functions as default and node ones as `_node()`
 - [:] circular based functions eg ill_circ_foreach() - skip for now
- [:] rb
 - [:] magic rb
 - [:] iterator (take example on da)
- [:] dsu
 - [:] cdsu reserve/up-front growth equivalent (no logs)
 - [:] make rdsu_grow(..., 0) a successful no-op like cdsu_grow
 - [:] validate the rewind log chain in rdsu_validate
- [:] tpm
 - [:] carsh jobs catch
 - [:] dispatch new jobs on spikes
 - [:] batch jobs on spikes
 - [:] heartbeat system

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

## Ressources
### DA : ressources

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

### DSU : ressources

### HP : ressources

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

### ILL : ressources

### LRU_CACHE : ressources

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

**Projects that motivate the cache**
- HTTP response cache for a small web service (ETag + TTL, stale-while-revalidate).
- Compiled-artifact/dependency cache for a build tool.
- DNS resolver cache (TTL heavy, negatives included).
- Page cache for a toy key-value store, or a memoization layer for a query planner.

### RB : ressources

### SPTR : ressources

### TLS : ressources

### TPM : ressources

