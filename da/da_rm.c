#include <string.h>

#include "dynamic_array.h"

// BUG: bug on the swap
// need to swap smallest between to_delete and right_gap
// rn if you try to delete something begger than right_gap shits starts to break

/**
 * \brief rm a single element while respecting the order of the da
 * \param vec pointer to the dynamic array
 * \param pos index of the element to remove
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_rm_order(t_da *vec, size_t pos) {
	if (!vec || vec->nb_e < pos) {
		LOG(C_MINI__DA_LOG_EARG);
		return (false);
	}
	if (vec->nb_e > pos + 1) {
		memcpy(da_at(vec, pos), da_at(vec, pos + 1),
		       vec->sz_e * (vec->nb_e - pos));
	}
	LOG(C_MINI__DA_LOG_RM, pos, 1UL, vec->nb_e, vec->nb_e - 1);
	vec->nb_e--;
	return (true);
}
/**
 * \brief rm elements [start, end[ while respecting the order of the da
 * \param vec pointer to the dynamic array
 * \param start index of the first element to remove
 * \param end index of the end element (exclusive)
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_rms_order(t_da *vec, size_t start, size_t end) {
	if (end < start || !vec || vec->nb_e < start) {
		LOG(C_MINI__DA_LOG_EARG);
		return (false);
	}
	if (vec->nb_e < end) end = vec->nb_e;
	if (vec->nb_e != end) {
		memcpy(da_at(vec, start), da_at(vec, end),
		       vec->sz_e * (vec->nb_e - end));
	}
	LOG(C_MINI__DA_LOG_RM, start, end - start, vec->nb_e,
	    vec->nb_e - (end - start));
	vec->nb_e -= end - start;
	return (false);
}

/**
 * \brief rm a single element (very fast) no garantee on order
 * \param vec pointer to the dynamic array
 * \param pos index of the element to remove
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_rm(t_da *vec, size_t pos) {
	if (!vec || vec->nb_e < pos) {
		LOG(C_MINI__DA_LOG_EARG);
		return (false);
	}
	if (pos != vec->nb_e) da_swap(vec, pos, vec->nb_e - 1);
	LOG(C_MINI__DA_LOG_RM, pos, 1UL, vec->nb_e, vec->nb_e - 1);
	vec->nb_e--;
	return (true);
}
/**
 * \brief rm elements (very fast) no garantee on order
 * \param vec pointer to the dynamic array
 * \param start index of the first element to remove
 * \param end index of the end element (exclusive)
 * \warning BUG: da_rms has a gap problem - check tests
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_rms(t_da *vec, size_t start, size_t end) {
	if (end < start || !vec || vec->nb_e < start) {
		LOG(C_MINI__DA_LOG_EARG);
		return (false);
	}
	if (vec->nb_e < end) end = vec->nb_e;
	if (vec->nb_e != end) {
		memcpy(da_at(vec, start), da_at(vec, vec->nb_e - (end - start)),
		       vec->sz_e * (end - start));
	}
	LOG(C_MINI__DA_LOG_RM, start, end - start, vec->nb_e,
	    vec->nb_e - (end - start));
	vec->nb_e -= end - start;
	return (true);
}
