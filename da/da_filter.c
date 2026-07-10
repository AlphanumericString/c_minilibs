#include "dynamic_array.h"

// NOTE: note optimisation non n-> no order_rm, since we pass on all elements
//		 cost: we destroy order

/**
 * \brief filter elements keeping those that match condition
 * \param da pointer to the dynamic array
 * \param c callback to test elements
 * \param u_data user data for callback
 * \param deletor callback to delete removed elements
 * \return true on success, false otherwise
 * \note filter might not keep order to keep order use n variant
**/
bool da_filter(t_da *da, t_callback c, void *u_data, t_callback deletor) {
	if (!c || !da || !da->nb_e) return (false);
	for (size_t i = 0; i < da->nb_e; i++) {
		if (!c(da_at(da, i), u_data)) {
			if (deletor) deletor(da_at(da, i), u_data);
			da_rm(da, i);
			i--;
		}
	}
	return (true);
}

/**
 * \brief filter first n elements keeping those that match condition
 * \param da pointer to the dynamic array
 * \param n number of elements to process
 * \param c callback to test elements
 * \param u_data user data for callback
 * \param deletor callback to delete removed elements
 * \return true on success, false otherwise
**/
bool da_nfilter(t_da *da, size_t n, t_callback c, void *u_data,
                t_callback deletor) {
	if (!c || !da || !da->nb_e) return (false);
	if (!n) return (true);
	if (n > da->nb_e) n = da->nb_e;
	for (size_t i = 0; i < n; i++) {
		if (!c(da_at(da, i), u_data)) {
			if (deletor) deletor(da_at(da, i), u_data);
			da_rm_order(da, i);
			i--;
			n--;
		}
	}
	return (true);
}

/**
 * \brief filter elements removing those that match condition
 * \param da pointer to the dynamic array
 * \param c callback to test elements
 * \param u_data user data for callback
 * \param deletor callback to delete removed elements
 * \return true on success, false otherwise
 * \note filter might not keep order to keep order use n variant
**/
bool da_filter_out(t_da *da, t_callback c, void *u_data, t_callback deletor) {
	if (!c || !da || !da->nb_e) return (false);
	for (size_t i = 0; i < da->nb_e; i++) {
		if (c(da_at(da, i), u_data)) {
			if (deletor) deletor(da_at(da, i), u_data);
			da_rm(da, i);
			i--;
		}
	}
	return (true);
}

/**
 * \brief filter first n elements removing those that match condition
 * \param da pointer to the dynamic array
 * \param n number of elements to process
 * \param c callback to test elements
 * \param u_data user data for callback
 * \param deletor callback to delete removed elements
 * \return true on success, false otherwise
**/
bool da_nfilter_out(t_da *da, size_t n, t_callback c, void *u_data,
                    t_callback deletor) {
	if (!c || !da || !da->nb_e) return (false);
	if (!n) return (true);
	if (n > da->nb_e) n = da->nb_e;
	for (size_t i = 0; i < n; i++) {
		if (c(da_at(da, i), u_data)) {
			if (deletor) deletor(da_at(da, i), u_data);
			da_rm_order(da, i);
			i--;
			n--;
		}
	}
	return (true);
}
