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

## What's missing / faults of the project
`common_base` : Module are described as independant yet all depends on these.
even though this is only for base defines and nothing complex this shoots the
"independance" of modules right in the foot.
`da` : It's described as a primitive and it feels like it. Nothing unique under
the sun and it shows. Lack distinc algorithms or function to be notable.
`dsu` : Log allocation are **the** slowest piece of the rdsu yet logs info are
not batched inside. Makes new linking have an unknown time and slows drasticly
the structure with malloc lookups (even if mitigated by reserve nodes this is
not enough)
`hp` : Not enough functions. The 'heap_up' and 'heap_down' accepting to run in
both min/max direction is ending up costing more in maintainer sanity than it's
worth.
`ill` : After all that not one circular list? No api consistency reguarding
head, elem function not implemented leading to user having to call exposed node
elem in their structs each time.
`lru_cache` : Time in ms? really? The structure try to merge an LRU and a TTL
cache, it utlimately succeed but the code/definitions endup feeling mushed and
hodgepoged together rather than a cohesive whole.
`rb` : not enough functions, feels restrictive. no multi threaded tests to
tests with the classic setup of producer-consumer thread.
`sptr` : generics render the lib unusable by older version of c and doesn't
bring "that" much to compensate. c is **not** a generic language. period.
Unique and other smart pointer implemented cant guard against byte-copy
rendering this inherently moot if not used as pointer.
`tls` : no hashmapt over infinitely growing vtls neuter the capacity and
utility of the struct in real use cases. ftls can only bind one single struct
rendering it's own use case quite small unless you re-comp with multiple name
and a bit of surgery (Xmacros would've helped).
`tpm` : doesn't have job batching, even though thread count is atomic thread
number is never changed, no repporting if job crashes, heartbeat system not
implemented, futures not implemented.

## Modules presentation

### `common_base`
Shared, zero-dependency building blocks: generic function-pointer typedefs
(callbacks, comparators, duplicators, destructors), the debug logging macros,
small math helpers and the test harness (malloc/clock wrappers used to inject
allocation and timing failures). **Skill:** designing reusable interfaces for
generic C and building testability into the code from day one.

### `da` — Dynamic Array
A compile-time-generic growable array (`void *` + element size) with the full
toolbox: create/dup/cat, get/set/add/remove, grow/shrink, sort, reverse, fold,
filter, find, apply and array conversion. **Skill:** manual memory management
and growth strategies, cache-friendly contiguous storage, and writing algorithms
generically over raw bytes.

### `dsu` — Disjoint Set Union
Two complementary flavors: `cdsu` (compact, array-based, path compression and
rank) and `rdsu` (rewindable, logging every union so the structure can be rolled
back), plus conversion routines between them. **Skill:** amortized complexity
analysis (near-constant find/union), union by rank, and designing a
persistent/undoable structure on top of a destructive one.

### `hp` — Heap
A binary min/max heap driven entirely by a user comparator, supporting create,
push, pop, push-pop, build-from-array, for-each and validity checks.
**Skill:** implicit tree layouts in arrays, sift-up/sift-down reasoning, and
heapify-from-array O(n) construction.

### `ill` — Intrusive Linked Lists
Two variants — singly (`ill`) and doubly (`idl`) linked — where nodes are
embedded *inside* the user's own structs, addressed via an offset, with
push/pop/append/prepend/insert/remove/reverse and list predicates.
**Skill:** intrusive containers, `offsetof`-style pointer arithmetic, aliasing
the allocation of the payload and its links, and O(1) splicing.

### `lru_cache` — Least Recently Used Cache (with TTL)
A cache whose capacity bound is enforced by recency ordering (a doubly linked
list) and whose entries can also expire by time, using a min-heap indexed by
TTL; supports hit/miss/expiry statistics and per-entry destructors.
**Skill:** combining multiple data structures (hash buckets + linked list +
heap) where each node lives in several indexes at once, and reasoning about
eviction policy trade-offs.

### `rb` — Round (Ring) Buffer
A fixed-capacity FIFO spanning a wrap-around buffer, with an optional
`stdatomic` build for lock-free single-producer / single-consumer use.
**Skill:** modular index arithmetic, wrap-around edge cases, and an
introduction to atomics and lock-free reasoning.

### `sptr` — Smart Pointers
Reference-counted pointer types in the spirit of C++ (`uniq`, `owned`,
`strong`, `weak`) backed by a shared metadata "box", with atomic refcounts,
weak counts and destructor ownership; a `_Generic` macro layer gives type-safe
creation/conversion between the categories. **Skill:** reference counting,
ownership models, atomic refcount semantics and thread-safety, and building
type-generic APIs with `_Generic`.

### `tls` — Thread Local Storage
Three approaches to the same problem: `ctls` (type-safe storage of base C
types), `vtls` (voided/generic key-value storage of arbitrary blobs) and `ftls`
(fastest, compile-time-typed struct with zero lookup overhead) — all built over
`pthread_key_t`. **Skill:** pthread TLS, the speed/flexibility trade-off
between compile-time and runtime typing, and constructor/destructor-driven
lifetime management.

### `tpm` — Thread Pool Manager
A work-stealing thread pool: a shared job queue with mutex/condition-variable
sleep-wake, per-thread job caches, recurring jobs, statistics and a lifecycle
(stop/wait). **Skill:** producer/consumer concurrency, condition variables,
work-stealing, thread lifecycle and graceful shutdown.

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
