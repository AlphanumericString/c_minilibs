#include <stdlib.h>

#define C_MINI__RDSU_INTERNAL
#include "rewindable_disjointed_set_union.h"

/**
 * \brief pop the most recent log from the rewind log
 * \param rdsu the rdsu to pop from
 * \return the popped log or NULL if the rewind log is empty
 * \ingroup rdsu
**/
t_cm_rdsu_log *rdsui_log_pop_rewind(t_cm_rdsu *rdsu) {
	t_cm_rdsu_log *ret;

	if (!rdsu || !rdsu->change_log) return (NULL);
	ret              = rdsu->change_log;
	rdsu->change_log = ret->nxt;
	ret->nxt         = NULL;
	return (ret);
}
/**
 * \brief push a log onto the rewind log
 * \param rdsu the rdsu to push to
 * \param entry the log to push
 * \ingroup rdsu
**/
void rdsui_log_push_rewind(t_cm_rdsu *rdsu, t_cm_rdsu_log *entry) {
	entry->nxt       = rdsu->change_log;
	rdsu->change_log = entry;
}
/**
 * \brief pop the most recent log from the reserved logs
 * \param rdsu the rdsu to pop from
 * \return the popped log or NULL if the reserve is empty
 * \ingroup rdsu
**/
t_cm_rdsu_log *rdsui_log_pop_reserved(t_cm_rdsu *rdsu) {
	t_cm_rdsu_log *ret;

	if (!rdsu || !rdsu->reserved_logs) return (NULL);
	ret                 = rdsu->reserved_logs;
	rdsu->reserved_logs = ret->nxt;
	ret->nxt            = NULL;
	rdsu->reserved_logs_nb--;
	return (ret);
}
/**
 * \brief push a log onto the reserved logs
 * \param rdsu the rdsu to push to
 * \param entry the log to push
 * \ingroup rdsu
**/
void rdsui_log_push_reserved(t_cm_rdsu *rdsu, t_cm_rdsu_log *entry) {
	entry->nxt          = rdsu->reserved_logs;
	rdsu->reserved_logs = entry;
	rdsu->reserved_logs_nb++;
}
/**
 * \brief get a log ready to be used, reusing the reserve first
 * \param rdsu the rdsu to get a log for
 * \return a log or NULL if none could be reused/allocated
 * \ingroup rdsu
**/
t_cm_rdsu_log *rdsui_log_get(t_cm_rdsu *rdsu) {
	t_cm_rdsu_log *ret;

	if (!rdsu) return (NULL);
	ret = rdsui_log_pop_reserved(rdsu);
	if (ret) return (ret);
	return ((t_cm_rdsu_log *)calloc(sizeof(t_cm_rdsu_log), 1));
}
/**
 * \brief free every log currently held by the rdsu
 * \param rdsu the rdsu to clear the logs of
 * \ingroup rdsu
**/
void rdsui_log_clear(t_cm_rdsu *rdsu) {
	t_cm_rdsu_log *entry;

	if (!rdsu) return;
	while (rdsu->change_log) {
		entry            = rdsu->change_log;
		rdsu->change_log = entry->nxt;
		free(entry);
	}
	while (rdsu->reserved_logs) {
		entry               = rdsu->reserved_logs;
		rdsu->reserved_logs = entry->nxt;
		free(entry);
	}
	rdsu->reserved_logs_nb = 0;
}
