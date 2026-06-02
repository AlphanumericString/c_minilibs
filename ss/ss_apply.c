#include "./sparse_set.h"

// apply
bool  ss_apply(t_ss *ss, void (*func)(void *elem)) {
	size_t	i;

	if (!ss || !func)
		return (false);
	for (i = 0; i < ss->nb_elem; i++)
		func(ss_at(ss, i));
	return (true);
}
bool  ss_apply_it(t_ss *ss, void (*func)(void *elem, size_t pos, bool last)) {
	size_t	i;

	if (!ss || !func)
		return (false);
	for (i = 0; i < ss->nb_elem; i++)
		func(ss_at(ss, i), i, (i + 1) == ss->nb_elem);
	return (true);
}
void *ss_fold(t_ss *ss, void *u_val, void (*func)(void *u_val, void *e_val)) {
	if (!ss || !func)
		return (false);
	return (NULL);
}
