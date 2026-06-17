#include "dynamic_array.h"

/**
 * \brief apply a function to each elements and gives position and boolean if
 * last
 * \param da pointer to the dynamic array
 * \param func function to apply
 * \param u_val user argument passed to the function
 * \return true if success, false if failure
 **/
void *da_apply_it(t_da *da, t_callback_it func, void *u_val) {
	return (da_napply_it(da, -1, func, u_val));
}

/**
 * \brief apply a function (func) to a da with a user argument(u_val). stops if
 * function reache the end or the callback returns false.
 * \param da pointer to the dynamic array
 * \param u_val user argument passed to the function
 * \param cb function to apply
 * \return the user argument (u_val)
 **/
void *da_apply(t_da *da, t_callback cb, void *u_val) {
	return (da_napply(da, -1, cb, u_val));
}

/**
 * \brief apply a function to the first n elements of the dynamic array
 * \param da pointer to the dynamic array
 * \param n number of elements to process
 * \param c function to apply
 * \param u_val user argument passed to the function
 * \return the user argument (u_val)
 **/
void *da_napply(t_da *da, size_t n, t_callback c, void *u_val) {
	size_t i;

	if (!da || !da->nb_e || !c || !n) return (u_val);
	if (n > da->nb_e) n = da->nb_e;
	i = 0;
	while (n--) {
		if (c(da_at(da, i++), u_val) != true) return (u_val);
	}
	return (u_val);
}

/**
 * \brief apply a function to the first n elements and gives position and boolean if last
 * \param da pointer to the dynamic array
 * \param n number of elements to process
 * \param func function to apply
 * \param u_val user argument passed to the function
 * \return the user argument (u_val)
 **/
void *da_napply_it(t_da *da, size_t n, t_callback_it func, void *u_val) {
	size_t i;

	if (!da || !func || !da->nb_e) return (u_val);
	if (n > da->nb_e) n = da->nb_e;
	i = 0;
	while (i < n) {
		if (func(da_at(da, i), u_val, i, i + 1 == n) != true) return (u_val);
		i++;
	}
	return (u_val);
}
