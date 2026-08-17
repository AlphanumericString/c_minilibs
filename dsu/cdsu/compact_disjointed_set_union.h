#ifndef C_MINI__CDSU
#define C_MINI__CDSU

#include <stdbool.h>
#include <stddef.h>

#include "common_base/log.h"
#ifdef C_MINI__TESTS

#	define C_MINI__CDSU_LOG_EARG   C_MINI__LOG_EARG
#	define C_MINI__CDSU_LOG_EALLOC C_MINI__LOG_EALLOC
#	define C_MINI__CDSU_LOG_FAIL   C_MINI__LOG_FAIL

#	define C_MINI__CDSU_LOG_SUCCESS C_MINI__LOG_SUCCESS

#endif // !C_MINI__TESTS

/**
 * \defgroup cdsu Compact DSU (disjointed set union)
**/

/**
 * \brief cdsu struct
 * \param parent list of ids for the parent of nodes
 * \param rank   rank of node closer to 0 = closer to source
 * \param components number of differnt sets present in the DSU
 * \param rp_single_alloc boolean on whether the rank and parent were allocated
 * from a continuous single alloc
 * \ingroup cdsu
**/
typedef struct s_compact_disjointed_set_union {
	unsigned int *parent;
	unsigned int *rank;

	size_t size;
	size_t components;
	bool   rp_single_alloc;
} t_cdsu;

t_cdsu *cdsu_new(size_t n);
t_cdsu *cdsu_new_from_array(size_t n, int *parents, int *rank);
t_cdsu *cdsu_init(t_cdsu *to_init, size_t n);
t_cdsu *cdsu_init_from_arrays(t_cdsu *to_init, size_t size, int *parent,
                              int *rank);

bool cdsu_grow(t_cdsu *to_init, size_t n);

bool cdsu_clr(t_cdsu *dsu);
void cdsu_destroy(t_cdsu *ds);
void cdsu_free(t_cdsu **ds);

// TODO: switch to 'const t_cdsu *'
int    cdsu_max_depth(t_cdsu dsu);
size_t cdsu_size(t_cdsu dsu);
size_t cdsu_nb_components(t_cdsu dsu);

bool cdsu_is_related(t_cdsu *ds, size_t x, size_t y);
bool cdsu_strict_is_related(t_cdsu ds, size_t x, size_t y);
bool cdsu_validate(t_cdsu dsu);

bool   cdsu_union(t_cdsu *ds, size_t x, size_t y);
size_t cdsu_find(t_cdsu *ds, size_t x);
size_t cdsu_strict_find(t_cdsu ds, size_t x);
size_t cdsu_compress_all(t_cdsu *ds);

#endif // !C_MINI__CDSU
