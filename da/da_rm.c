#include <string.h>

#include "dynamic_array.h"

// BUG: bug on the swap
// need to swap smallest between to_delete and right_gap
// rn if you try to delete something begger than right_gap shits starts to break

/**
 * \brief rm a single element while respecting the order of the da
 * \param da pointer to the dynamic array
 * \param pos index of the element to remove
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_rm_order(t_cm_da *da, size_t pos) {
	if (!da_is_valid(da) || da->nb_e < pos) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if (da->nb_e > pos + 1) {
		memmove(da_at(da, pos), da_at(da, pos + 1),
		        da->sz_e * (da->nb_e - pos));
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_RM, pos, (size_t)1UL, da->nb_e,
	       da->nb_e - 1)
	da->nb_e--;
	return (true);
}
/**
 * \brief rm elements [start, end[ while respecting the order of the da
 * \param da pointer to the dynamic array
 * \param start index of the first element to remove
 * \param end index of the end element (exclusive)
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_rms_order(t_cm_da *da, size_t start, size_t end) {
	if (!da_is_valid(da) || end < start || da->nb_e <= start) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if (da->nb_e < end) end = da->nb_e;
	if (da->nb_e != end) {
		memmove(da_at(da, start), da_at(da, end), da->sz_e * (da->nb_e - end));
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_RM, start, end - start, da->nb_e,
	       da->nb_e - (end - start))
	da->nb_e -= end - start;
	return (true);
}

/**
 * \brief rm a single element (very fast) no garantee on order
 * \param da pointer to the dynamic array
 * \param pos index of the element to remove
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_rm(t_cm_da *da, size_t pos) {
	if (!da_is_valid(da) || da->nb_e < pos) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if (pos != da->nb_e) da_swap(da, pos, da->nb_e - 1);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_RM, pos, (size_t)1UL, da->nb_e,
	       da->nb_e - 1)
	da->nb_e--;
	return (true);
}
/**
 * \brief rm elements (very fast) no garantee on order
 * \param da pointer to the dynamic array
 * \param start index of the first element to remove
 * \param end index of the end element (exclusive)
 * \warning BUG: da_rms has a gap problem - check tests
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_rms(t_cm_da *da, size_t start, size_t end) {
	if (end < start || !da_is_valid(da) || da->nb_e <= start) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if (da->nb_e < end) end = da->nb_e;
	if (da->nb_e != end) {
		memcpy(da_at(da, start), da_at(da, end), da->sz_e * (da->nb_e - end));
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_RM, start, end - start, da->nb_e,
	       da->nb_e - (end - start))
	da->nb_e -= end - start;
	return (true);
}
