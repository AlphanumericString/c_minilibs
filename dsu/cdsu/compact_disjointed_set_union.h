#ifndef C_MINI__CDSU
#define C_MINI__CDSU

#include <stdbool.h>
#include <stddef.h>

#ifdef C_MINI__TESTS
#	include <stdio.h>

#	ifndef LOG
#		define LOG(fmt, ...)                                                  \
			do {                                                               \
				printf("\e[33m[LOG]\e[0m: %s: " fmt "\n",                      \
				       __func__ __VA_OPT__(, ) __VA_ARGS__);                   \
				fflush(stdout);                                                \
			} while (0);
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...)                                                  \
			do {                                                               \
				printf("\e[36;1m[DBG]\e[0m: %s: " fmt "\n",                    \
				       __func__ __VA_OPT__(, ) __VA_ARGS__);                   \
				fflush(stdout);                                                \
			} while (0);
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS                                                        \
			do {                                                               \
				printf("\e[34;0m[PATH]\e[0m: %s:%d %s\n", __FILE__, __LINE__,  \
				       __FUNCTION__);                                          \
				fflush(stdout);                                                \
			} while (0);
#	endif // !DBG_POS

#	define C_MINI__CDSU_LOG_EARG   "Bad arguments."
#	define C_MINI__CDSU_LOG_EALLOC "Allocation failure."
#	define C_MINI__CDSU_LOG_FAIL   "Failure"

#	define C_MINI__CDSU_LOG_SUCCESS "Success"

#else // || C_MINI__TESTS

#	ifndef LOG
#		define LOG(fmt, ...) (void)0;
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...) (void)0;
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS (void)0;
#	endif // !DBG_POS

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

t_cdsu *cdsu_grow(t_cdsu *to_init, size_t n);

void cdsu_clr(t_cdsu *dsu);
void cdsu_destroy(t_cdsu *ds);
void cdsu_free(t_cdsu *ds);

int    cdsu_max_depth(t_cdsu dsu);
size_t cdsu_size(t_cdsu dsu);
size_t cdsu_nb_components(t_cdsu dsu);

bool cdsu_is_related(t_cdsu *ds, size_t x, size_t y);
bool cdsu_strict_is_related(t_cdsu ds, size_t x, size_t y);
bool cdsu_validate(t_cdsu dsu);

void   cdsu_union(t_cdsu *ds, size_t x, size_t y);
size_t cdsu_find(t_cdsu *ds, size_t x);
size_t cdsu_strict_find(t_cdsu ds, size_t x);
size_t cdsu_compress_all(t_cdsu *ds);

#endif // !C_MINI__CDSU
