#include "./dynamic_array.h"

/**
 * \brief apply a function to each elements
 * \param da pointer to the dynamic array
 * \param func function to apply to each element
 * \return true if success, false if failure
 */
bool da_apply(t_da *da, void (*func)(void *elem)) {
	size_t i;

	if (!da || !func || !da->nb_e) return (false);
	i = 0;
	while (i < da->nb_e)
		func(da_at(da, i++));
	return (true);
}

/**
 * \brief apply a function to each elements and gives position and boolean if last
 * \param da pointer to the dynamic array
 * \param func function to apply
 * \return true if success, false if failure
 */
bool da_apply_it(t_da *da, void (*func)(void *elem, size_t pos, bool last)) {
	size_t i;

	if (!da || !func || !da->nb_e) return (false);
	i = 0;
	while (i < da->nb_e) {
		func(da_at(da, i), i, i + 1 == da->nb_e);
		i++;
	}
	return (true);
}

/**
 * \brief apply a function (func) to a da with a user argument(u_val)
 * \param da pointer to the dynamic array
 * \param u_val user argument passed to the function
 * \param func function to apply
 * \return the user argument (u_val)
 */
void	*da_apply_with(t_da *da, void *u_val, void (*func)(void *u_val, void *e_val)){
	size_t i;

	i = 0;
	while (i < da->nb_e)
		func(u_val, da_at(da, i++));
	return (u_val);
}