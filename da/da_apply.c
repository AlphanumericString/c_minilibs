#include "dynamic_array.h"

/**
 * \brief apply a function to each elements and gives position and boolean if
 * last
 * \param da pointer to the dynamic array
 * \param func function to apply
 * \param u_val user argument passed to the function
 * \return true if success, false if failure
 * \ingroup da
**/
void *da_apply_it(t_da *da, t_cm_callback_it func, void *u_val) {
	void *r;

	if (!da_is_valid(da) || da_is_empty(da) || !func) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (u_val);
	}
	r = da_napply_it(da, -1, func, u_val);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (r);
}
/**
 * \brief apply a function (func) to a da with a user argument(u_val). stops if
 * function reache the end or the callback returns false.
 * \param da pointer to the dynamic array
 * \param u_val user argument passed to the function
 * \param cb function to apply
 * \return the user argument (u_val)
 * \ingroup da
**/
void *da_apply(t_da *da, t_cm_callback cb, void *u_val) {
	void *r;

	if (!da_is_valid(da) || da_is_empty(da) || !cb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (u_val);
	}
	r = da_napply(da, -1, cb, u_val);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (r);
}
/**
 * \brief apply a function to the first n elements of the dynamic array
 * \param da pointer to the dynamic array
 * \param n number of elements to process
 * \param c function to apply
 * \param u_val user argument passed to the function
 * \return the user argument (u_val)
 * \ingroup da
**/
void *da_napply(t_da *da, size_t n, t_cm_callback c, void *u_val) {
	size_t i;

	if (!da_is_valid(da) || da_is_empty(da) || !c || !n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (u_val);
	}
	if (n > da->nb_e) n = da->nb_e;
	for (i = 0; n && c(da_at(da, i++), u_val); n--)
		;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (u_val);
}
/**
 * \brief apply a function to the first n elements and gives position and
 * boolean if last
 * \param da pointer to the dynamic array
 * \param n number of elements to process
 * \param func function to apply
 * \param u_val user argument passed to the function
 * \return the user argument (u_val)
 * \ingroup da
**/
void *da_napply_it(t_da *da, size_t n, t_cm_callback_it func, void *u_val) {
	size_t i;

	if (!da_is_valid(da) || da_is_empty(da) || !func) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (u_val);
	}
	if (n > da->nb_e) n = da->nb_e;
	for (i = 0; i < n && func(da_at(da, i), u_val, i, i + 1 == n); i++)
		;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (u_val);
}
