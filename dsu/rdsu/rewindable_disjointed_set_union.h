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
#	define C_MINI__RDSU_LOG_EALLOC "Allocation failure."
#	define C_MINI__RDSU_LOG_ELOG   "No log to replay."
#	define C_MINI__RDSU_LOG_ELUN   "Unknown log."
#	define C_MINI__RDSU_LOG_ELC    "Log corruption."
#	define C_MINI__RDSU_LOG_FAIL   "Failure."

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

/**
 * \defgroup rdsu Rewindable DSU (Disjointed Set Union)
**/

#ifdef C_MINI__RDSU_INTERNAL

/**
 * @brief [INTERNAL] [DO NOT USE] the log type enum
 * \ingroup rdsu
**/
enum s_rdsu_log_type {
	RDSU_LOG_RANK,
	RDSU_LOG_PARENT,
};

typedef struct s_rdsu_log {
	struct s_rdsu_log *nxt;

	enum s_rdsu_log_type type;
	uint16_t             elem;
	uint16_t             p;
	uint16_t             r;
} t_rdsu_log;

#else // || C_MINI__RDSU_INTERNAL

/**
 * \brief the structure of a log entry in the rdsu
 * \param nxt pointer to next entry in the stack of log
 * \param type the type of entry (enum abstracted as int to not clutter LSP)
 * \param elem the elem whoms't we changed the parent of
 * \param p the previous parent value
 * \param r the rank that was modified if there was a rank change
 * \ingroup rdsu
**/
typedef struct s_rdsu_log {
	struct s_rdsu_log *nxt;

	int      type;
	uint16_t elem;
	uint16_t r;
	uint16_t p;
} t_rdsu_log;

#endif // !C_MINI__RDSU_INTERNAL

/**
 * \brief rdsu struct
 * \param parent list of ids for the parent of nodes
 * \param rank   rank of node closer to 0 = closer to source
 * \param components number of differnt sets present in the RDSU
 * \param rp_single_alloc boolean on whether the rank and parent were allocated
 * from a continuous single alloc
 * \param change_log the stack of entries/changes in the rdsu
 * \ingroup rdsu
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

bool rdsui_push_rewind(t_rdsu *rdsu, size_t elem, uint16_t value, uint16_t rank,
                       enum s_rdsu_log_type type);
t_rdsu_log *rdsui_peek_rewind(const t_rdsu *rdsu);
t_rdsu_log *rdsui_pop_rewind(t_rdsu *rdsu);

#endif // !C_MINI__RDSU_INTERNAL

#endif // !C_MINI__RDSU
