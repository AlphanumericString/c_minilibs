#ifndef C_MINI__RDSU
#define C_MINI__RDSU

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#include "common_base/log.h"

#ifdef C_MINI__TESTS

#	define C_MINI__RDSU_LOG_FAIL   C_MINI__LOG_FAIL
#	define C_MINI__RDSU_LOG_EARG   C_MINI__LOG_EARG
#	define C_MINI__RDSU_LOG_EALLOC C_MINI__LOG_EALLOC
#	define C_MINI__RDSU_LOG_ELOG   C_MINI__LOG_F_PREFIX "No log to replay."
#	define C_MINI__RDSU_LOG_ELUN   C_MINI__LOG_F_PREFIX "Unknown log."
#	define C_MINI__RDSU_LOG_ELC    C_MINI__LOG_F_PREFIX "Log corruption."

#	define C_MINI__RDSU_LOG_SUCCESS C_MINI__LOG_SUCCESS

#endif // !C_MINI__TESTS

/**
 * \defgroup rdsu Rewindable DSU (Disjointed Set Union)
**/

#ifdef C_MINI__RDSU_INTERNAL

#	ifndef C_MINI__RDSU_LOG_CHECK_FAST
#		define C_MINI__RDSU_LOG_CHECK_STRICT
#	endif

/**
 * @brief [INTERNAL] [DO NOT USE] the log type enum
 * \ingroup rdsu
**/
enum s_rdsu_log_type {
	RDSU_LOG_YESRANK,
	RDSU_LOG_NAYRANK,
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
 * \typedef t_rdsu_log
 * \brief the structure of a log entry in the rdsu (rewindable disjointed set
 * union)
 * \param nxt pointer to next entry in the stack of logs
 * \param type the type of entry (enum abstracted as int to not clutter LSP)
 * \param elem the elem whose parent we changed
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
 * \typedef t_rdsu
 * \brief rdsu struct represents a rewindable dsu (disjointed set union)
 * \param parent list of ids for the parent of nodes
 * \param rank   rank of a node, closer to 0 = closer to the source
 * \param size number of nodes the dsu can hold
 * \param components number of different sets present in the RDSU
 * \param rp_single_alloc boolean on whether the rank and parent were allocated
 * from a single contiguous allocation
 * \param change_log the stack of entries/changes in the rdsu
 * \ingroup rdsu
 * \note rdsu are not "auto" compacted, furthermore compacting impacts logs and
 * invalidates said logs. This means that unless you're sure you don't need to
 * rewind, compacting is heavily disincentivized.
**/
typedef struct s_rewindable_disjointed_set_union {
	uint16_t *parent;
	uint16_t *rank;

	size_t size;
	size_t components;
	bool   rp_single_alloc;

	t_rdsu_log *change_log;
} t_rdsu;

t_rdsu *rdsu_new(size_t n);
t_rdsu *rdsu_new_from_arrays(size_t n, uint16_t *parents, uint16_t *rank);
t_rdsu *rdsu_init(t_rdsu *to_init, size_t n);
t_rdsu *rdsu_init_from_arrays(t_rdsu *to_init, size_t size, uint16_t *parent,
                              uint16_t *rank);

bool rdsu_grow(t_rdsu *to_init, size_t n);

// change to bool return
void rdsu_clr(t_rdsu *dsu);
void rdsu_destroy(t_rdsu *ds);
void rdsu_free(t_rdsu **ds);

int    rdsu_max_depth(const t_rdsu *dsu);
size_t rdsu_size(const t_rdsu *dsu);
size_t rdsu_nb_components(const t_rdsu *dsu);

bool rdsu_is_related(const t_rdsu *ds, size_t x, size_t y);
bool rdsu_validate(const t_rdsu *dsu);

bool     rdsu_union(t_rdsu *ds, size_t x, size_t y);
uint16_t rdsu_find(const t_rdsu *ds, size_t x);
size_t   rdsu_compress_all(t_rdsu *ds);

t_rdsu_log *rdsu_save_state(const t_rdsu *rdsu);
bool        rdsu_rewind(t_rdsu *rdsu);
bool        rdsu_rewind_to_state(t_rdsu *rdsu, t_rdsu_log *lg);
void        rdsu_rewind_clear(t_rdsu *rdsu);

#ifdef C_MINI__RDSU_INTERNAL

bool        rdsui_push_rewind(t_rdsu *rdsu, t_rdsu_log *entry);
t_rdsu_log  rdsui_init_log(size_t elem, uint16_t value, uint16_t rank,
                           enum s_rdsu_log_type type);
t_rdsu_log *rdsui_peek_rewind(const t_rdsu *rdsu);
t_rdsu_log *rdsui_pop_rewind(t_rdsu *rdsu);

#endif // !C_MINI__RDSU_INTERNAL

#endif // !C_MINI__RDSU
