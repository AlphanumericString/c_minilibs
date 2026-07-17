#include <stddef.h>
#include <stdlib.h>
#define C_MINI__RDSU_INTERNAL
#include "rewindable_disjointed_set_union.h"

// usefull log check
static inline bool is_log_valid(const t_rdsu *rdsu, const t_rdsu_log *log) {
	return (rdsu && log && rdsu->size > log->elem &&
	        rdsu->size > log->value.p &&
	        (log->type == RDSU_LOG_RANK || log->type == RDSU_LOG_PARENT));
}

// simple stack handling
bool rdsui_push_rewind(t_rdsu *rdsu, size_t elem, size_t value,
                       enum s_rdsu_log_type type) {
	t_rdsu_log *entry;
	t_rdsu_log  tentative_entry;

	tentative_entry = (t_rdsu_log){
	    .type = type, .elem = elem, .value.p = value, .nxt = rdsu->change_log};
	if (is_log_valid(rdsu, &tentative_entry) == false) return (false);
	if (type != RDSU_LOG_RANK && type != RDSU_LOG_PARENT) return (false);
	entry = calloc(sizeof(*entry), 1);
	if (!entry) return (false);
	*entry           = tentative_entry;
	rdsu->change_log = entry;
	return (true);
}
t_rdsu_log *rdsui_peek_rewind(const t_rdsu *rdsu) {
	return (rdsu->change_log);
}
t_rdsu_log *rdsui_pop_rewind(t_rdsu *rdsu) {
	t_rdsu_log *ret;

	if (!rdsu) return (NULL);
	ret = rdsui_peek_rewind(rdsu);
	if (!ret) return (NULL);
	rdsu->change_log = ret->nxt;
	ret->nxt         = NULL;
	return (ret);
}
// end

// rewind util functions
static bool execute_rewind(t_rdsu *rdsu, t_rdsu_log lg) {

	if (is_log_valid(rdsu, &lg) == false) return (false);
	switch (lg.type) {
	case (RDSU_LOG_RANK):
		rdsu->rank[lg.elem] = lg.value.r;
		break;
	case (RDSU_LOG_PARENT):
		rdsu->parent[lg.elem] = lg.value.p;
		rdsu->components++;
		break;
	default:
		return (false); // error
	}
	return (true);
}

//	-- exposed api
/**
 * \brief this function allows to save the state of the dsu via a log pointer
 * \param rdsu the rdsu to save the state of
 * \return the current log pointer (can be null if no logs). If bad parameters
 * are given (eg: null rdsu) returns null.
 **/
t_rdsu_log *rdsu_save_state(t_rdsu *rdsu) {
	t_rdsu_log *r;

	if (!rdsu) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return (NULL);
	}
	r = rdsui_peek_rewind(rdsu);
	LOG(C_MINI__RDSU_LOG_SUCCESS);
	return (r);
}
/**
 * \brief this function allows one to rewind a rdsu to a previous state.
 * \param rdsu the rdsu to rewind
 * \return true if the opperation was successful otherwise false.
 **/
bool rdsu_rewind(t_rdsu *rdsu) {
	t_rdsu_log *entry = rdsui_pop_rewind(rdsu);
	bool        ret;

	if (!rdsu) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return (false);
	}
	entry = rdsui_pop_rewind(rdsu);
	if (!entry) {
		LOG(C_MINI__RDSU_LOG_ELOG);
		return (false);
	}
	ret = execute_rewind(rdsu, *entry);
	if (ret == false) {
		LOG(C_MINI__RDSU_LOG_ELC);
		return (ret);
	}
	if (entry->type == RDSU_LOG_RANK) {
		free(entry);
		entry = rdsui_pop_rewind(rdsu);
		if (!entry) return (ret);
		ret = execute_rewind(rdsu, *entry);
	}
	free(entry);
	if (ret == true) {
		LOG(C_MINI__RDSU_LOG_SUCCESS);
		return (true);
	}
	LOG(C_MINI__RDSU_LOG_ELC);
	return (false);
}
/**
 * \brief allow someone to rewind up to a specific state using a log pointer
 * \param rdsu the rdsu to rewind
 * \param lg the log pointer to rewind to
 * \return true if the opperation was successfull, false otherwise
 * \note as log is a linked list it's implicitely trusted that lg is part of the
 * log, in case this is false it'll rewind up to null and return false.
 **/
bool rdsu_rewind_to_state(t_rdsu *rdsu, t_rdsu_log *lg) {
	t_rdsu_log *entry;
	bool        res;

	if (!rdsu || !lg) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return (false);
	}
	if (!rdsu->change_log) {
		LOG(C_MINI__RDSU_LOG_ELOG);
		return (false);
	}
	res = true;
	while (rdsu->change_log != lg && res) {
		entry = rdsui_pop_rewind(rdsu);
		res   = execute_rewind(rdsu, *entry);
		free(entry);
	}
	if (res == false) {
		LOG(C_MINI__RDSU_LOG_ELC);
		return (false);
	}
	if (res == true && rdsu->change_log != lg) {
		LOG(C_MINI__RDSU_LOG_ELUN);
		return (false);
	}
	LOG(C_MINI__RDSU_LOG_SUCCESS);
	return (true);
}
/**
 * \brief clear the list of entries in the rdsu rewind log
 * \param rdsu the rdsu to clear the log off
 **/
void rdsu_rewind_clear(t_rdsu *rdsu) {
	if (!rdsu) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return;
	}
	while (rdsu->change_log)
		free(rdsui_pop_rewind(rdsu));
	LOG(C_MINI__RDSU_LOG_SUCCESS);
}
