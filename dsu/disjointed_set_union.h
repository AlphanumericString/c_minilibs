#ifndef C_MINI__DSU
#	define C_MINI__DSU

#	include <stddef.h>
#	include <stdbool.h>

#	ifdef C_MINI__TESTS
#		include <stdio.h>

#		ifndef LOG
#			define LOG(fmt, ...)                                              \
				do {                                                           \
					printf("\e[33m[LOG]\e[0m: %s: " fmt "\n",                  \
					       __func__ __VA_OPT__(, ) __VA_ARGS__);               \
					fflush(stdout);                                            \
				} while (0);
#		endif // !LOG

#		ifndef DBG
#			define DBG(fmt, ...)                                              \
				do {                                                           \
					printf("\e[36;1m[DBG]\e[0m: %s: " fmt "\n",                \
					       __func__ __VA_OPT__(, ) __VA_ARGS__);               \
					fflush(stdout);                                            \
				} while (0);
#		endif // !DBG

#		ifndef DBG_POS
#			define DBG_POS                                                    \
				do {                                                           \
					printf("\e[34;0m[PATH]\e[0m: %s:%d %s\n", __FILE__,        \
					       __LINE__, __FUNCTION__);                            \
					fflush(stdout);                                            \
				} while (0);
#		endif // !DBG_POS

#define C_MINI__DSU_LOG_EARG    "Bad arguments."
#define C_MINI__DSU_LOG_EALLOC  "Allocation faillure."
#define C_MINI__DSU_LOG_FAIL    "Faillure"

#define C_MINI__DSU_LOG_SUCCESS "Success"

#	else // || C_MINI__TESTS

#		ifndef LOG
#			define LOG(fmt, ...) (void)0;
#		endif // !LOG

#		ifndef DBG
#			define DBG(fmt, ...) (void)0;
#		endif // !DBG

#		ifndef DBG_POS
#			define DBG_POS (void)0;
#		endif // !DBG_POS

#	endif // !C_MINI__TESTS

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
void dsu_free(t_dsu *ds);

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

#endif // !C_MINI__DSU
