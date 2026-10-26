// set -> bool: false if alloc needed on path and failed (inode or fnode),
//	override if other val present
// store -> bool: same as set but return prev value at index i
//
// get ->  bool: load ptr|val at index i in *out param, if nothing
//  at i false.
// load -> bool: same as get but return pos in xa
//
// for_each -> bool: true while elem to process (use iter_fwd and iter_bwd)
//
// for_each_mark -> bool:true while elem to process(use iter_fwd and iter_bwd)
//
// swap -> bool:false if nothing at idx a or idx b
//
// reserve -> bool: create mem path for idx i, true on success
//
// alloc -> void*: return mem region for object of size s, fail if too big
//	(XA_NB_SLOTS * sizeof(void *) == max)
//
// alloc_no_alloc -> void*: same as alloc but dont create new trie if no space,
//	try to fit in what slots the xarray already has
//
// alloc_no_grow -> void*: same as alloc but dont create new indexes, instad try
//	to re-use lower bound indexes and alloc inode/fnode if needed, null if
//	needed alloc fails
//
// insert -> bool: same as set but fails if elem already present there
//
// erase -> bool: true if elem was present at index i
