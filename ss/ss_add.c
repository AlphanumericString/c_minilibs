#include <string.h>
#include "./sparse_set.h"

bool  ss_add(t_ss *ss, void *elem){
	void	*pos;

	if (!ss || !ss->data || !ss->indexes || ss->nb_elem == ss->cappacity)
		return (false);
	ss->nb_elem++;
	pos = ss_at(ss, ss->nb_elem - 1);
	memcpy(pos, elem, ss->sz_elem);
	return (true);
}
bool  ss_add_array(t_ss *ss, void *arr, size_t nb_e) {
	if (!ss || !ss->data || !ss->indexes || (ss->nb_elem + nb_e) == ss->cappacity)
		return (false);
	for (size_t i = 0; i < nb_e; i++)
		ss_add(ss, (arr + (i * ss->sz_elem)));
	return (true);
}
