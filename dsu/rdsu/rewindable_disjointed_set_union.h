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
enum s_cm_rdsu_log_type {
	RDSU_LOG_YESRANK,
	RDSU_LOG_NAYRANK,
};

typedef struct s_cm_rdsu_log {
	struct s_cm_rdsu_log *nxt;

	enum s_cm_rdsu_log_type type;
	uint16_t                elem;
	uint16_t                p;
	uint16_t                r;
} t_cm_rdsu_log;

#else // || C_MINI__RDSU_INTERNAL

/**
 * \typedef t_cm_rdsu_log
 * \brief the structure of a log entry in the rdsu (rewindable disjointed set
 * union)
 * \param nxt pointer to next entry in the stack of logs
 * \param type the type of entry (enum abstracted as int to not clutter LSP)
 * \param elem the elem whose parent we changed
 * \param p the previous parent value
 * \param r the rank that was modified if there was a rank change
 * \ingroup rdsu
**/
typedef struct s_cm_rdsu_log {
	struct s_cm_rdsu_log *nxt;

	int      type;
	uint16_t elem;
	uint16_t r;
	uint16_t p;
} t_cm_rdsu_log;

#endif // !C_MINI__RDSU_INTERNAL

/**
 * \typedef t_cm_rdsu
 * \brief rdsu struct represents a rewindable dsu (disjointed set union)
 * \param parent list of ids for the parent of nodes
 * \param rank   rank of a node, closer to 0 = closer to the source
 * \param size number of nodes the dsu can hold
 * \param components number of different sets present in the RDSU
 * \param rp_single_alloc boolean on whether the rank and parent were allocated
 * from a single contiguous allocation
 * \param change_log the stack of entries/changes in the rdsu
 * \param reserved_logs the stack of logs that where popped and can be re-used
 * for future linkings (reduces alloc/free churn on do/undo cycles)
 * \param reserved_logs_nb number of logs currently available in
 * reserved_logs
 * \ingroup rdsu
 * \note rdsu are not "auto" compacted, furthermore compacting impacts logs and
 * invalidates said logs. This means that unless you're sure you don't need to
 * rewind, compacting is heavily disincentivized.
**/
typedef struct s_cm_rewindable_disjointed_set_union {
	uint16_t *parent;
	uint16_t *rank;

	size_t size;
	size_t components;
	bool   rp_single_alloc;

	t_cm_rdsu_log *change_log;
	t_cm_rdsu_log *reserved_logs;
	size_t         reserved_logs_nb;
} t_cm_rdsu;

t_cm_rdsu *rdsu_new(size_t n);
t_cm_rdsu *rdsu_new_from_arrays(size_t n, uint16_t *parents, uint16_t *rank);
t_cm_rdsu *rdsu_init(t_cm_rdsu *to_init, size_t n);
t_cm_rdsu *rdsu_init_from_arrays(t_cm_rdsu *to_init, size_t size,
                                 uint16_t *parent, uint16_t *rank);

bool rdsu_grow(t_cm_rdsu *to_init, size_t n);
bool rdsu_reserve(t_cm_rdsu *rdsu, size_t n);

// change to bool return
void rdsu_clr(t_cm_rdsu *dsu);
void rdsu_destroy(t_cm_rdsu *ds);
void rdsu_free(t_cm_rdsu **ds);

int    rdsu_max_depth(const t_cm_rdsu *dsu);
size_t rdsu_size(const t_cm_rdsu *dsu);
size_t rdsu_reserved_logs_nb(const t_cm_rdsu *dsu);
size_t rdsu_nb_components(const t_cm_rdsu *dsu);

bool rdsu_is_related(const t_cm_rdsu *ds, size_t x, size_t y);
bool rdsu_validate(const t_cm_rdsu *dsu);

bool     rdsu_union(t_cm_rdsu *ds, size_t x, size_t y);
uint16_t rdsu_find(const t_cm_rdsu *ds, size_t x);
size_t   rdsu_compress_all(t_cm_rdsu *ds);

t_cm_rdsu_log *rdsu_save_state(const t_cm_rdsu *rdsu);
bool           rdsu_rewind(t_cm_rdsu *rdsu);
bool           rdsu_rewind_to_state(t_cm_rdsu *rdsu, const t_cm_rdsu_log *lg);
void           rdsu_rewind_clear(t_cm_rdsu *rdsu);

#ifdef C_MINI__RDSU_INTERNAL

void           rdsui_log_push_rewind(t_cm_rdsu *rdsu, t_cm_rdsu_log *entry);
t_cm_rdsu_log *rdsui_log_pop_rewind(t_cm_rdsu *rdsu);
void           rdsui_log_push_reserved(t_cm_rdsu *rdsu, t_cm_rdsu_log *entry);
t_cm_rdsu_log *rdsui_log_pop_reserved(t_cm_rdsu *rdsu);
t_cm_rdsu_log *rdsui_log_get(t_cm_rdsu *rdsu);
void           rdsui_log_clear(t_cm_rdsu *rdsu);

#endif // !C_MINI__RDSU_INTERNAL

#endif // !C_MINI__RDSU
