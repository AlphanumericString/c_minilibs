#include "dynamic_array.h"

// NOTE: note optimisation non n-> no order_rm, since we pass on all elements
//		 cost: we destroy order

/**
 * \brief filter elements keeping those that match condition
 * \param da pointer to the dynamic array
 * \param filter callback to test elements, true:keep false:out
 * \param arg user data for callback
 * \param del callback to delete removed elements
 * \return true on success, false otherwise
 * \note filter might not keep order to keep order use n variant
 * \ingroup da
**/
bool da_filter(t_da *da, t_callback filter, void *arg, t_callback del) {
	if (!filter || !da_is_valid(da) || !da->nb_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	for (size_t i = 0; i < da->nb_e; i++) {
		if (!filter(da_at(da, i), arg)) {
			if (del) del(da_at(da, i), arg);
			da_rm(da, i);
			i--;
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}
/**
 * \brief filter first n elements keeping those that match condition
 * \param da pointer to the dynamic array
 * \param n number of elements to process
 * \param filter callback to test elements, true:keep false:out
 * \param data user data for callback
 * \param del callback to delete removed elements
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_nfilter(t_da *da, size_t n, t_callback filter, void *data,
                t_callback del) {
	if (!filter || !da_is_valid(da) || !da->nb_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if (n > da->nb_e) n = da->nb_e;
	for (size_t i = 0; i < n; i++) {
		if (!filter(da_at(da, i), data)) {
			if (del) del(da_at(da, i), data);
			da_rm_order(da, i);
			i--;
			n--;
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}

/**
 * \brief filter elements removing those that match condition
 * \param da pointer to the dynamic array
 * \param filter callback to test elements against, true:out false:keep
 * \param data user data for callback to test with
 * \param del callback to delete removed elements
 * \return true on success, false otherwise
 * \note filter might not keep order to keep order use n variant
 * \ingroup da
**/
bool da_filter_out(t_da *da, t_callback filter, void *data, t_callback del) {
	if (!filter || !da_is_valid(da) || !da->nb_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	for (size_t i = 0; i < da->nb_e; i++) {
		if (filter(da_at(da, i), data)) {
			if (del) del(da_at(da, i), data);
			da_rm(da, i);
			i--;
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}
/**
 * \brief filter first n elements removing those that match condition
 * \param da pointer to the dynamic array
 * \param n number of elements to process
 * \param filter callback to test elements, true:out false:keep
 * \param data user data for callback
 * \param del callback to delete removed elements
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_nfilter_out(t_da *da, size_t n, t_callback filter, void *data,
                    t_callback del) {
	if (!filter || !da_is_valid(da) || !da->nb_e) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if (!n) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
		return (true);
	}
	if (n > da->nb_e) n = da->nb_e;
	for (size_t i = 0; i < n; i++) {
		if (filter(da_at(da, i), data)) {
			if (del) del(da_at(da, i), data);
			da_rm_order(da, i);
			i--;
			n--;
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}
