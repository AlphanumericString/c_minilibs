#include "./sparse_set.h"

// convert
void *ss_to_array(t_ss *ss) {
	void *ret;

	if (!ss) return (NULL);
	ret = malloc(ss->nb_elem * ss->nb_elem);
	return (ss_to_loc_array(ss, ret));
}
void *ss_to_loc_array(t_ss *ss, void *arr) {
	if (!ss || !arr) return (NULL);
	for (size_t i = 0; i < ss->nb_elem; i++)
		ss_get(ss, i, (arr + i * ss->sz_elem));
	return (arr);
}
