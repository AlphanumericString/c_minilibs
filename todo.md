# TODO
general todo for the project
- `x` done
- `~` running
- `:` paused
- ` ` not started
General todo:
- [x] current modules
- [:] refactor
 - [ ] lruc .time refactor
 - [ ] da->check todo impl, add functs to module
 - [ ] dsu-> rdsu -> reserve func
 - [ ] hp-> more funcs
 - [ ] ill->circular lists, api consistency around head, elem centric funcs
 - [ ] lruc->ms to ns, make a separate lru only cache similar to dsu/
 - [ ] rb->example w producer/consumer setup, add functs to module
 - [ ] rptr->package _generic in ifdef blocks and check compile ok for c older version
 - [ ] tls->vtls->hmaps. tls->ftls->xmacros for more than 1 struct
 - [ ] tpm->crash catch, job batching on stress conditions, atomic thrd nb - increase/remove thrd control handle, auto increase thrd nb on stress
- [:] coverage
- [:] impl
- [:] new modules
    - [:] add ressources for todo new modules

## CURRENT MODULES

### FIXES/REFACTOR
- [:] lruc refactor
    - [ ] rewrite lru_cache so that ttl is a size_t ns_gap in kv that is either -1 (no ttl) or a non null ns duration.
    - [ ] rename current .time to .time_emitted
    - [ ] setttl add ttl value to ns_gap instead of in .time_emitted
    - [ ] when get/has/other add .time_emitted + .ns_gap, if < now then out else ok
    - [ ] rename function get to try_get -> try, if ok ret val else ret NULL.
    - [ ] add get function `get("key", klen, &fun)` if key ok then ret val, else if !key in lruc then call fun & set kv.
    - [ ] add getttl func `getttl("key", klen, &fun, ttl_ns)`
        if (!ttl_ns) -> call new version get();
        if (!time || !ns_gap || .ns_gap + .time < now) call `fun`, update val, reset last_set, update .ns_gap
- [:] refactors
transferred from the readme.md "what's missing / faults of the project" section
 - [ ] da->check todo impl, add functs to module
 - [ ] dsu-> rdsu -> reserve func
 - [ ] hp-> more funcs
 - [ ] ill->circular lists, api consistency around head, elem centric funcs
 - [ ] lruc->ms to ns, make a separate lru only cache similar to dsu/
 - [ ] rb->example w producer/consumer setup, add functs to module
 - [ ] rptr->package _generic in ifdef blocks and check compile ok for c older version
 - [ ] tls->vtls->hmaps. tls->ftls->xmacros for more than 1 struct
 - [ ] tpm->crash catch, job batching on stress conditions, atomic thrd nb - increase/remove thrd control handle, auto increase thrd nb on stress

### COVERAGE
- [:] multi threaded tests (needs re-comp for #ifdef branches)
 - [ ] sptr
 - [ ] rb
- [:] da
 - [ ] iter
 - [ ] oscilation around boudary with auto_shrink

### DOC
- [:] **MARKETING PART**
 - [ ] da
  - [ ] Growth factor comparison (1.5x, 2x, fixed increment): realloc count and time.
  - [ ] Baselines: raw `realloc` loop and C++ `std::vector::push_back`.
  - [ ] Auto-shrink on vs off in an oscillating workload.
  - [ ] `make bench` that outputs CSV, plus a gnuplot/matplotlib script. Commit the PNGs to the README.
- [:] **POLISH**
 - [ ] README with graphs or visible benchmarks.
  - [ ] graphs
  - [ ] da
   - [ ] A small real use case, such as a tokenizer or CSV parser built on the library.
   - [ ] Small-buffer optimization, only if the benchmarks justify it. Write its tests afterward, since it changes the allocation paths.
  - [ ] dsu
  - [ ] rb
  - [ ] hp
  - [ ] ill
  - [ ] lruc
  - [ ] sptr
  - [ ] tls
  - [ ] tpm

### IMPLEMENTATION
- [:] add github CICD rules for merge w main
 - [ ] add cppcheck step
- [:] makefile rules to recomp w multi threaded and merge into coverage
 - [ ] add multi threaded tests
  - [ ] restrict to specific modules
 - [ ] add multi thread tests to coverage mapping
- [:] ill
 - [ ] elem based functions
 - [ ] set elem based functions as default and node ones as `_node()`
- [:] rb
 - [ ] magic rb
- [:] da
 - [ ] iter (eg: `iter(holder, start, end, &elem_it)`) -1 for end, otherwise start included end not, if end oob / -1 end included
 - [ ] map(eg: `map(holder, fn, out_elem_size)`)
 - [ ] nmap(eg: `nmap(holder, n, fn, out_elem_size)`)
 - [ ] ifdef auto shrink (path on pop/rm) with (len <= cap/4 -> len = cap/2)
  - [ ] shrink on rm
  - [ ] shrink on pop
- [:] tpm
 - [ ] carsh jobs catch
 - [ ] dispatch new jobs on spikes
 - [ ] batch jobs on spikes
 - [ ] heartbeat system

note: new module paused. finish tpm first.
note: new module paused. finish school project first.

## NEW MODULES - WHOLE MODULE PAUSED
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
- Chandler Carruth, "Tuning C++: Benchmarks, and CPUs, and Compilers! Oh My!" (CppCon 2015), for making your benchmarks credible
- Andrei Alexandrescu, "std::allocator Is to Allocation what std::vector Is to Vexation" (CppCon 2015)
- Matt Kulukundis, "Designing a Fast, Efficient, Cache-friendly Hash Table, Step by Step" (CppCon 2017)

### DSU : ressources

### HP : ressources

### ILL : ressources

### LRU_CACHE : ressources

### RB : ressources

### SPTR : ressources

### TLS : ressources

### TPM : ressources

