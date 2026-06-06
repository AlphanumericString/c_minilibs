#ifndef PRIV_DSU

#	include <stddef.h>
#	include <stdio.h>
#	include <stdlib.h>
#	include <stdbool.h>

/**
 * \brief dsu struct
 * \param parent list of ids for the parent of nodes
 * \param rank   rank of node closer to 0 = closer to source
 * \param components number of differnt sets present in the DSU
 * \param rp_single_alloc boolean on whether the rank and parent were allocated
 * from a continuous single alloc
**/
typedef struct s_disjointed_set_union {
	unsigned int *parent;
	unsigned int *rank;
	size_t        size;
	size_t        components;
	bool          rp_single_alloc;
} t_dsu;

// create
t_dsu *dsu_new(size_t n);
t_dsu *dsu_new_from_array(size_t n, int *parents, int *rank);
t_dsu *dsu_init(t_dsu *to_init, size_t n);
t_dsu *dsu_init_from_arrays(t_dsu *to_init, size_t size, int *parent,
                            int *rank);

// grow
t_dsu *dsu_grow(t_dsu *to_init, size_t n);

// destroy
void dsu_clr(t_dsu *dsu);
void dsu_destroy(t_dsu *ds);

// getter
int    dsu_max_depth(t_dsu dsu);
size_t dsu_size(t_dsu dsu);
size_t dsu_nb_components(t_dsu dsu);

// querry
bool dsu_is_related(t_dsu *ds, size_t x, size_t y);
bool dsu_strict_is_related(t_dsu ds, size_t x, size_t y);
bool dsu_validate(t_dsu dsu);

// links
void   dsu_union(t_dsu *ds, size_t x, size_t y);
size_t dsu_find(t_dsu *ds, size_t x);
size_t dsu_strict_find(t_dsu ds, size_t x);
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
