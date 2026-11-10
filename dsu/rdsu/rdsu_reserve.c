#include <stdlib.h>

#define C_MINI__RDSU_INTERNAL
#include "rewindable_disjointed_set_union.h"

/**
 * \brief reserve log entries for future linkings
 * \param rdsu the rdsu to reserve logs for
 * \param n number of log entries that must be available in the reserve
 * \return true if at least n logs are available, false otherwise
 * \note this does not change the dsu links or size, it only pre-allocates the
 * rewind logs used by rdsu_union. On failure the reserve is left unchanged.
 * \ingroup rdsu
**/
bool rdsu_reserve(t_cm_rdsu *rdsu, size_t n) {
	t_cm_rdsu_log *entry;
	t_cm_rdsu_log *old_head;

	if (!rdsu || !rdsu->parent || !rdsu->rank) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (false);
	}
	if (rdsu->reserved_logs_nb >= n) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
		return (true);
	}
	old_head = rdsu->reserved_logs;
	while (rdsu->reserved_logs_nb < n) {
		entry = (t_cm_rdsu_log *)calloc(sizeof(*entry), 1);
		if (!entry) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RDSU_LOG_EALLOC)
			while (rdsu->reserved_logs != old_head)
				free(rdsui_log_pop_reserved(rdsu));
			return (false);
		}
		rdsui_log_push_reserved(rdsu, entry);
	}
	CM_LOG(C_MINI__LEVEL_INFO, "Reserve logs (rdsu:%p, reserved:%zu)",
	       (void *)rdsu, rdsu->reserved_logs_nb)
	return (true);
}
