# TODO

General todo
- [ ] current
    - [ ] fixes
    - [ ] coverage
    - [ ] doc
    - [ ] impl
- [ ] new modules
    - [ ] add ressources for todo new modules

## CURRENT MODULES
### FIXES
- [ ] tpm
 - [ ] where the fuck are we loosing jobs?!
### COVERAGE
- [ ] tpm - paused (find lost jobs)
- [ ] multi threaded tests (needs re-comp for #ifdef branches)
 - [ ] sptr
 - [ ] rb
### DOC
- [x] Add README.md
 - [x] why c_minilib
 - [x] presentation of modules
 - [x] what i learned
- [x] doxygen ok?
 - [ ] comment why rb grow create new rb
- [ ] example directory w small example per module
 - [ ] da
 - [ ] dsu
 - [ ] hp
 - [ ] ill
 - [ ] lru_cache
 - [ ] rb
 - [ ] sptr
 - [ ] tls
 - [ ] tpm - paused (find lost jobs)
### IMPLEMENTATION
- [ ] add github CICD rules for merge w main
 - [ ] how
 - [ ] where
 - [ ] how many $/h ?
- [ ] makefile rules to recomp w multi threaded and merge into coverage
 - [ ] exec tests
- [ ] ill
 - [ ] elem based functions
 - [ ] set elem based functions as default and node ones as `_node()`
- [ ] rename all struct/typedef all w `[s|t|e]_cm_` in front for colision resiliency (skip functions name, unlikely)

note: new module paused. finish tpm first.
note: new module paused. finish school project first.

## NEW MODULES
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
