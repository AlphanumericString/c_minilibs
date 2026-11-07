# c_minilibs

A collection of small, dependency-free, header-friendly C libraries.

Each module is an independent mini-library that re-implements a classic data
structure or systems primitive from scratch. The goal is not to ship a polished
framework, but to learn the *why* behind the tools we use every day: exercise
generic C, manual memory management, intrusive designs, concurrency and the
toolchain around them.

Every module ships with its own test suite (allocation/clock failure injection,
coverage, valgrind and static analysis) and can be amalgamated into a single
stb-style header.

## Why

Because the best way to understand a data structure or a threading primitive is
to build it, break it, and then test it again.
This also serves as a primitive to other project hence the `remove_module` rule
in the makefile for quickly removin un-needed dependencies.

## Building

```sh
make                 # compile every module into its own lib<module>.a
make tests           # build every module's test binaries
make tests_run       # build and run tests under valgrind
make coverage        # llvm-cov HTML report in coverage/
make static_analysis # cppcheck over every module
make documentation   # doxygen HTML in documentation/
make single_file MODULE=<module>   # amalgamate one module into single/da.h
```

Targets are module-aware through `MODULE=<name>`, and debug logging is enabled
with `DEBUG_LVL=<level>`.

**Skills:** writing a non-trivial Makefile (metaprogramming the module build
from a template, per-module object trees and dependency files), driving the
LLVM coverage toolchain (`-fprofile-instr-generate` -> `profdata` -> `llvm-cov`),
running tests under valgrind, static analysis with cppcheck, doxygen docs, and
making optional paths (debug logging, fault injection, atomics, threaded tests)
selectable through macros rather than edits.

## Modules presentation

### `common_base`
Shared, zero-dependency building blocks: generic function-pointer typedefs
(callbacks, comparators, duplicators, destructors), the debug logging macros,
small math helpers and the test harness (malloc/clock wrappers used to inject
allocation and timing failures). **Skill:** designing reusable interfaces for
generic C and building testability into the code from day one.
**Complexity:** not applicable — utility code, no container operations.

### `da` — Dynamic Array
A compile-time-generic growable array (`void *` + element size) with the full
toolbox: create/dup/cat, get/set/add/remove, grow/shrink, sort, reverse, fold,
filter, find, apply and array conversion. **Skill:** manual memory management
and growth strategies, cache-friendly contiguous storage, and writing algorithms
generically over raw bytes.
**Complexity:** `da_add` amortized O(1) (1.5x growth), `da_at`/`da_set` O(1),
middle insert/remove O(n), grow/shrink O(n) (realloc + copy), find/filter/apply
O(n), sort O(n log n).

### `dsu` — Disjoint Set Union
Two complementary flavors: `cdsu` (compact, array-based, path compression and
rank) and `rdsu` (rewindable, logging every union so the structure can be rolled
back), plus conversion routines between them. **Skill:** amortized complexity
analysis (near-constant find/union), union by rank, and designing a
persistent/undoable structure on top of a destructive one.
**Complexity:** `find`/`link` amortized O(α(n)) (inverse Ackermann; path
compression + union by rank). Rewind is O(1) per undone operation.

### `hp` — Heap
A binary min/max heap driven entirely by a user comparator, supporting create,
push, pop, push-pop, build-from-array, for-each and validity checks.
**Skill:** implicit tree layouts in arrays, sift-up/sift-down reasoning, and
heapify-from-array O(n) construction.
**Complexity:** push/pop/push-pop O(log n); at/peek O(1); build-from-array O(n)
(heapify); foreach O(n).

### `ill` — Intrusive Linked Lists
Two variants — singly (`ill`) and doubly (`idl`) linked — where nodes are
embedded *inside* the user's own structs, addressed via an offset, with
push/pop/append/prepend/insert/remove/reverse and list predicates.
**Skill:** intrusive containers, `offsetof`-style pointer arithmetic, aliasing
the allocation of the payload and its links, and O(1) splicing.
**Complexity:** push/pop O(1); insert/remove given a node O(1); reverse O(n);
search O(n); sort O(n log n).

### `lru_cache` — Least Recently Used Cache (with TTL)
A cache whose capacity bound is enforced by recency ordering (a doubly linked
list) and whose entries can also expire by time, using a min-heap indexed by
TTL; supports hit/miss/expiry statistics and per-entry destructors.
**Skill:** combining multiple data structures (hash buckets + linked list +
heap) where each node lives in several indexes at once, and reasoning about
eviction policy trade-offs.
**Complexity:** `get`/`put`/`has` expected O(1) (hash bucket lookup); TTL
insert/update O(log n) (min-heap); cleanup/eviction O(k log n) for k expiries;
foreach O(n).

### `rb` — Round (Ring) Buffer
A fixed-capacity FIFO spanning a wrap-around buffer, with an optional
`stdatomic` build for lock-free single-producer / single-consumer use.
**Skill:** modular index arithmetic, wrap-around edge cases, and an
introduction to atomics and lock-free reasoning.
**Complexity:** push/pop/peek/at O(1); apply/map/filter O(n); grow O(n) (single
realloc + copy). Atomics build is lock-free SPSC.

### `sptr` — Smart Pointers
Reference-counted pointer types in the spirit of C++ (`uniq`, `owned`,
`strong`, `weak`) backed by a shared metadata "box", with atomic refcounts,
weak counts and destructor ownership; a `_Generic` macro layer gives type-safe
creation/conversion between the categories. **Skill:** reference counting,
ownership models, atomic refcount semantics and thread-safety, and building
type-generic APIs with `_Generic`.
**Complexity:** creation/deref/copy/drop O(1); refcount changes are atomic RMW
O(1).

### `tls` — Thread Local Storage
Three approaches to the same problem: `ctls` (type-safe storage of base C
types), `vtls` (voided/generic key-value storage of arbitrary blobs) and `ftls`
(fastest, compile-time-typed struct with zero lookup overhead) — all built over
`pthread_key_t`. **Skill:** pthread TLS, the speed/flexibility trade-off
between compile-time and runtime typing, and constructor/destructor-driven
lifetime management.
**Complexity:** `ftls` O(1) (compile-time offset, zero lookup); `ctls` O(n)
linear scan bounded by `C_MINI__CTLS_CAP`; `vtls` O(n) linear scan over the
key list.

### `tpm` — Thread Pool Manager
A work-stealing thread pool: a shared job queue with mutex/condition-variable
sleep-wake, per-thread job caches, recurring jobs, statistics and a lifecycle
(stop/wait). **Skill:** producer/consumer concurrency, condition variables,
work-stealing, thread lifecycle and graceful shutdown.
**Complexity:** enqueue/dequeue O(1) under the queue mutex; steal O(threads) per
attempt. No O(1) job cancellation/lookup yet (hash map not implemented).

## Layout

```
common_base/   shared typedefs, logging, math, test harness
<module>/      module headers (*.h) and sources (*.c)
tests/         per-module test suites
scripts/       amalgamate.sh, remove_module.sh
Makefile       build, test, coverage, docs and static analysis
```

## License

See [LICENSE](LICENSE).
