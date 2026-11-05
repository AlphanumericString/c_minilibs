#ifndef C_MINI__CDSU
#define C_MINI__CDSU

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
 * \typedef t_cm_cdsu
 * \brief cdsu struct represents a compact dsu (disjointed set union)
 * \param parent list of ids for the parent of nodes
 * \param rank   rank of a node, closer to 0 = closer to the source
 * \param size number of nodes the dsu can hold
 * \param components number of different sets present in the DSU
 * \param rp_single_alloc boolean on whether the rank and parent were allocated
 * from a single contiguous allocation
 * \ingroup cdsu
**/
typedef struct s_cm_compact_disjointed_set_union {
	uint16_t *parent;
	uint16_t *rank;

	size_t size;
	size_t components;
	bool   rp_single_alloc;
} t_cm_cdsu;

t_cm_cdsu *cdsu_new(size_t n);
t_cm_cdsu *cdsu_new_from_array(size_t n, uint16_t *parents, uint16_t *rank);
t_cm_cdsu *cdsu_init(t_cm_cdsu *to_init, size_t n);
t_cm_cdsu *cdsu_init_from_arrays(t_cm_cdsu *to_init, size_t size,
                                 uint16_t *parent, uint16_t *rank);

bool cdsu_grow(t_cm_cdsu *to_init, size_t n);

bool cdsu_clr(t_cm_cdsu *dsu);
void cdsu_destroy(t_cm_cdsu *ds);
void cdsu_free(t_cm_cdsu **ds);

int    cdsu_max_depth(const t_cm_cdsu *dsu);
size_t cdsu_size(const t_cm_cdsu *dsu);
size_t cdsu_nb_components(const t_cm_cdsu *dsu);

bool cdsu_is_related(t_cm_cdsu *ds, size_t x, size_t y);
bool cdsu_strict_is_related(const t_cm_cdsu *ds, size_t x, size_t y);
bool cdsu_validate(const t_cm_cdsu *dsu);

bool   cdsu_union(t_cm_cdsu *ds, size_t x, size_t y);
size_t cdsu_find(t_cm_cdsu *ds, size_t x);
size_t cdsu_strict_find(const t_cm_cdsu *ds, size_t x);
size_t cdsu_compress_all(t_cm_cdsu *ds);

#endif // !C_MINI__CDSU
