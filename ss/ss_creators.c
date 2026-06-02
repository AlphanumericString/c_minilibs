#include "./sparse_set.h"

t_ss *ss_new(size_t sz_elem) {
	return (ss_init((t_ss *)calloc(sizeof(t_ss), 1), sz_elem));
}
t_ss *ss_init(t_ss *ss, size_t sz_elem) {
	if (!ss || !sz_elem) return (NULL);
	return (ss_ninit(ss, PSS_NB_ELEM_DFLT, sz_elem));
}
t_ss *ss_ninit(t_ss *ss, size_t nb_elem, size_t sz_elem) {
	void *data;
	uint *index;

	if (!ss || !nb_elem || !sz_elem) return (NULL);
	data  = calloc(sz_elem, nb_elem);
	index = malloc(sizeof(*index) * nb_elem);
	return (ss_from_array(ss, data, index, nb_elem, sz_elem));
}
t_ss *ss_from_array(t_ss *ss, void *array, uint *index_array, size_t nb_elem,
                    size_t sz_elem) {
	size_t i;

	if (!ss || !array || !index_array || !nb_elem || !sz_elem) return (NULL);
	ss->data      = array;
	ss->indexes   = index_array;
	ss->nb_elem   = 0;
	ss->cappacity = nb_elem;
	ss->sz_elem   = sz_elem;
	i             = 0;
	while (i < ss->cappacity) {
		ss->indexes[i] = i;
		i++;
	}
	return (ss);
}
