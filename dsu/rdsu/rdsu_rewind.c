#include <stdlib.h>

#define C_MINI__RDSU_INTERNAL
#include "rewindable_disjointed_set_union.h"

static inline bool is_log_valid(const t_rdsu *rdsu, const t_rdsu_log *log) {
	return (rdsu->size > log->elem && rdsu->size > log->p &&
	        ((log->type == RDSU_LOG_YESRANK && rdsu->size > log->r) ||
	         log->type == RDSU_LOG_NAYRANK));
}

t_rdsu_log rdsui_init_log(size_t elem, uint16_t value, uint16_t rank,
                          enum s_rdsu_log_type type) {

	return ((t_rdsu_log){
	    .type = type, .elem = elem, .p = value, .r = rank, .nxt = NULL});
}

bool rdsui_push_rewind(t_rdsu *rdsu, t_rdsu_log *entry) {
	if (!rdsu || !entry || is_log_valid(rdsu, entry) == false) return (false);
	entry->nxt       = rdsu->change_log;
	rdsu->change_log = entry;
	return (true);
}
t_rdsu_log *rdsui_peek_rewind(const t_rdsu *rdsu) {
	return (rdsu->change_log);
}
t_rdsu_log *rdsui_pop_rewind(t_rdsu *rdsu) {
	t_rdsu_log *ret;

	if (!rdsu || !rdsu->parent || !rdsu->rank) return (NULL);
	ret = rdsui_peek_rewind(rdsu);
	if (!ret) return (NULL);
	rdsu->change_log = ret->nxt;
	ret->nxt         = NULL;
	return (ret);
}

static bool execute_rewind(t_rdsu *rdsu, t_rdsu_log lg) {

	if (is_log_valid(rdsu, &lg) == false) return (false);
	rdsu->parent[lg.elem] = lg.p;
	if (lg.type == RDSU_LOG_YESRANK) {
		rdsu->rank[lg.r]--;
	}
	rdsu->components++;
	return (true);
}

#ifdef C_MINI__RDSU_LOG_CHECK_STRICT
// horrible performance.
static inline bool is_log_in_book(const t_rdsu *rdsu, t_rdsu_log *log) {
	t_rdsu_log *entry = rdsu->change_log;

	for (entry = rdsu->change_log; entry && entry != log; entry = entry->nxt)
		;
	return (entry == log);
}
#endif

/**
 * \brief this function allows to save the state of the dsu via a log pointer
 * \param rdsu the rdsu to save the state of
 * \return the current log pointer (can be null if no logs). If bad parameters
 * are given (eg: null rdsu) returns null.
 * \ingroup rdsu
**/
t_rdsu_log *rdsu_save_state(t_rdsu *rdsu) {
	t_rdsu_log *r;

	if (!rdsu) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (NULL);
	}
	r = rdsui_peek_rewind(rdsu);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (r);
}
/**
 * \brief this function allows one to rewind a rdsu to a previous state.
 * \param rdsu the rdsu to rewind
 * \return true if the opperation was successful otherwise false.
 * \ingroup rdsu
**/
bool rdsu_rewind(t_rdsu *rdsu) {
	t_rdsu_log *entry = rdsui_pop_rewind(rdsu);
	bool        ret;

	if (!rdsu || !rdsu->parent || !rdsu->rank) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (false);
	}
	if (!entry) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_ELOG)
		return (false);
	}
	ret = execute_rewind(rdsu, *entry);
	if (ret == false) {
		entry->nxt       = rdsu->change_log;
		rdsu->change_log = entry;
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_ELC)
		return (ret);
	}
	free(entry);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (true);
}
/**
 * \brief allow someone to rewind up to a specific state using a log pointer
 * \param rdsu the rdsu to rewind
 * \param lg the log pointer to rewind to
 * \return true if the opperation was successfull, false otherwise
 * \note as log is a linked list it's implicitely trusted that lg is part of the
 * log, in case this is false it'll rewind up to null and return false.
 * \ingroup rdsu
**/
bool rdsu_rewind_to_state(t_rdsu *rdsu, t_rdsu_log *lg) {
	t_rdsu_log *entry;
	bool        res;

	if (!rdsu) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (false);
	}
	if (!rdsu->change_log || !lg || !is_log_valid(rdsu, lg)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_ELOG)
		return (false);
	}
#ifdef C_MINI__RDSU_LOG_CHECK_STRICT
	if (!is_log_in_book(rdsu, lg)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_ELUN)
		return (false);
	}
#endif
	res = true;
	while (rdsu->change_log != lg && res) {
		entry = rdsui_pop_rewind(rdsu);
		res   = execute_rewind(rdsu, *entry);
		if (!res) {
			entry->nxt       = rdsu->change_log;
			rdsu->change_log = entry;
		} else free(entry);
	}
	if (res == false) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_ELC)
		return (false);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (true);
}
/**
 * \brief clear the list of entries in the rdsu rewind log
 * \param rdsu the rdsu to clear the log off
 * \ingroup rdsu
**/
void rdsu_rewind_clear(t_rdsu *rdsu) {
	if (!rdsu) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return;
	}
	while (rdsu->change_log)
		free(rdsui_pop_rewind(rdsu));
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
}
