#ifndef C_MINI__RDSU
#define C_MINI__RDSU

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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

#	define C_MINI__RDSU_LOG_EARG   "Bad arguments."
#	define C_MINI__RDSU_LOG_EALLOC "Allocation faillure."
#	define C_MINI__RDSU_LOG_ELOG   "No log to replay."
#	define C_MINI__RDSU_LOG_ELUN   "Unknown log."
#	define C_MINI__RDSU_LOG_ELC    "Log corruption."
#	define C_MINI__RDSU_LOG_FAIL   "Faillure."

#	define C_MINI__RDSU_LOG_SUCCESS "Success."

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

#ifdef C_MINI__RDSU_INTERNAL

enum s_rdsu_log_type {
	RDSU_LOG_RANK,
	RDSU_LOG_PARENT,
	//	RDSU_LOG_SAVEPOINT // optimized as reference to log elem so no need
};

typedef struct s_rdsu_log {
	struct s_rdsu_log *nxt;

	enum s_rdsu_log_type type;
	uint16_t             elem;
	union {
		uint16_t r; // rank
		uint16_t p; // parent
	} value;
} t_rdsu_log;

#else // || C_MINI__RDSU_INTERNAL

typedef struct s_rdsu_log {
	struct s_rdsu_log *nxt;

	int type;
	int elem;
	union {
		int rank;
		int parent;
	} log_info;
} t_rdsu_log;

#endif // !C_MINI__RDSU_INTERNAL

/**
 * \brief dsu struct
 * \param parent list of ids for the parent of nodes
 * \param rank   rank of node closer to 0 = closer to source
 * \param components number of differnt sets present in the DSU
 * \param rp_single_alloc boolean on whether the rank and parent were allocated
 * from a continuous single alloc
 **/
typedef struct s_rewindable_disjointed_set_union {
	uint16_t *parent;
	uint16_t *rank;

	size_t size;
	size_t components;
	bool   rp_single_alloc;

	t_rdsu_log *change_log;
} t_rdsu;

// create
t_rdsu *rdsu_new(size_t n);
t_rdsu *rdsu_new_from_array(size_t n, uint16_t *parents, uint16_t *rank);
t_rdsu *rdsu_init(t_rdsu *to_init, size_t n);
t_rdsu *rdsu_init_from_arrays(t_rdsu *to_init, size_t size, uint16_t *parent,
                              uint16_t *rank);

// grow
t_rdsu *rdsu_grow(t_rdsu *to_init, size_t n);

// destroy
void rdsu_clr(t_rdsu *dsu);
void rdsu_destroy(t_rdsu *ds);
void rdsu_free(t_rdsu *ds);

// getter
int    rdsu_max_depth(t_rdsu dsu);
size_t rdsu_size(t_rdsu dsu);
size_t rdsu_nb_components(t_rdsu dsu);

// querry
bool rdsu_is_related(t_rdsu *ds, size_t x, size_t y);
bool rdsu_validate(t_rdsu dsu);

// links
void     rdsu_union(t_rdsu *ds, size_t x, size_t y);
uint16_t rdsu_find(t_rdsu *ds, size_t x);
size_t   rdsu_compress_all(t_rdsu *ds);

// rewind
t_rdsu_log *rdsu_save_state(t_rdsu *rdsu);
bool        rdsu_rewind(t_rdsu *rdsu);
bool        rdsu_rewind_to_state(t_rdsu *rdsu, t_rdsu_log *lg);
void        rdsu_rewind_clear(t_rdsu *rdsu);

#ifdef C_MINI__RDSU_INTERNAL

bool        rdsui_push_rewind(t_rdsu *rdsu, size_t elem, size_t value,
                              enum s_rdsu_log_type type);
t_rdsu_log *rdsui_peek_rewind(const t_rdsu *rdsu);
t_rdsu_log *rdsui_pop_rewind(t_rdsu *rdsu);

#endif // !C_MINI__RDSU_INTERNAL

#endif // !C_MINI__RDSU
