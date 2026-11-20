#include "hp_internal.h"

/**
 * \brief Rebuild a min heap in place from its current elements
 * \param h heap pointer
 * \return true on success, false otherwise
 * \note complexity is O(n)
 * \ingroup hp
**/
bool hp_min_heapify(t_cm_hp *h) {
	if (!hp_is_valid(h)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	hpi_heapify(h, false);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}

/**
 * \brief Rebuild a max heap in place from its current elements
 * \param h heap pointer
 * \return true on success, false otherwise
 * \note complexity is O(n)
 * \ingroup hp
**/
bool hp_max_heapify(t_cm_hp *h) {
	if (!hp_is_valid(h)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	hpi_heapify(h, true);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}
