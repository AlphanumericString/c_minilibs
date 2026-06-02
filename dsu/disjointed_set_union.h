#ifndef PRIV_DSU

#	include <stddef.h>
#	include <stdio.h>
#	include <stdlib.h>
#	include <stdbool.h>

/**
 * disjointed set union:
 *
 * set of nodes with easy query to check if nodes are in the same network
**/

/// @brief dsu struct
/// @param parent list of ids for the parent of nodes
/// @param rank   rank of node closer to 0 = closer to source
/// @param components number of differnt sets present in the DSU
/// @param rp_single_alloc boolean on whether the rank and parent were
///		allocated from a continuous single alloc
typedef struct s_disjointed_set_union {
	unsigned int *parent;
	unsigned int *rank;
	size_t        size;
	size_t        components;
	bool          rp_single_alloc;
} t_dsu;

// constructors

/* init by allocating both the return and the internals */
t_dsu *dsu_new(size_t n);
/* init by allocating a t_dsu and using provided values */
t_dsu *dsu_new_from_array(size_t n, int *parents, int *rank);
/* init by allocating the internals */
t_dsu *dsu_init(t_dsu *to_init, size_t n);
/* init t_dsu using provided values (no allocs) */
t_dsu *dsu_init_from_arrays(t_dsu *to_init, size_t size, int *parent,
                            int *rank);
/**
 * grow the dsu by N values
 * (warning: will realocate. if you used local buffers durring construction
 * via from_arrays methods do not use).
 *
**/
t_dsu *dsu_grow(t_dsu *to_init, size_t n);

// destructor

/* reset structure to a clean init state */
void dsu_clr(t_dsu *dsu);
/* frees internal memory */
void dsu_destroy(t_dsu *ds);

// getters

/* returns the max depth of the tree */
int    dsu_max_depth(t_dsu dsu);
/* returns the number of elems in the tree */
size_t dsu_size(t_dsu dsu);
/* return the number of distinc separate sets in the dsu */
size_t dsu_nb_components(t_dsu dsu);

// querry

/* check if 2 node have the same ancestor (uses find so compression) */
bool   dsu_is_related(t_dsu *ds, size_t x, size_t y);
/* check if 2 node have the same ancestor (uses strct_find so no compression) */
bool   dsu_strict_is_related(t_dsu ds, size_t x, size_t y);
/* check if dsu is valid and w/out errors */
bool   dsu_validate(t_dsu dsu);

// links
/* links 2 elements */
void   dsu_union(t_dsu *ds, size_t x, size_t y);
/* search parent of x and compress parent */
size_t dsu_find(t_dsu *ds, size_t x);
/* ONLY search, no compression */
size_t dsu_strict_find(t_dsu ds, size_t x);
/* try to compress all elem sequentially */
size_t dsu_compress_all(t_dsu *ds);

// TODO:
//	COMPONENT_COUNT		- tests check if it's accurate

// TODO:
//	ROLL_BACK			- add stack(pop/push/peek) + flag
//						- if try to find_compression when flag says roll_back
//							-> strict_find redirect || error
//						- if try to is_related when flag says roll_back
//							-> strict_is_related redirect || error
//						- add bool undo(t_dsu *ds)
//							-> true if replayed

#	define PRIV_DSU
#endif // !PRIV_DSU
