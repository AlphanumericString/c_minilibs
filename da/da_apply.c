#include "./dynamic_array.h"

bool da_apply(t_da *da, void (*func)(void *elem)) {
	size_t i;

	if (!da || !func || !da->nb_e) return (false);
	i = 0;
	while (i < da->nb_e)
		func(da_at(da, i++));
	return (true);
}

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

void	*da_apply_with(t_da *da, void *u_val, void (*func)(void *u_val, void *e_val)){
	size_t i;

	i = 0;
	while (i < da->nb_e)
		func(u_val, da_at(da, i++));
	return (u_val);
}
