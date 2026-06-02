#include "./dynamic_array.h"

t_da *da_new(size_t sz_elem) {
	t_da *ret;

	ret = (t_da *)calloc(sizeof(t_da), 1);
	return (da_init(ret, sz_elem));
}

t_da *da_init(t_da *ret, size_t sz_elem) {
	void *data;

	if (!ret) return (NULL);
	data = (void *)calloc(sz_elem, PVEC_NB_ELEM_DFLT);
	return (da_from_array(ret, data, PVEC_NB_ELEM_DFLT, sz_elem));
}

t_da *da_ninit(t_da *ret, size_t n, size_t sz_elem) {
	void *data;

	if (!ret) return (NULL);
	data = (void *)calloc(sz_elem, n);
	return (da_from_array(ret, data, n, sz_elem));
}

t_da *da_from_array(t_da *ret, void *data, size_t nb_elem, size_t sz_elem) {
	if (!ret || !data) return (ret);
	ret->array = data;
	ret->sz_e  = sz_elem;
	ret->nb_e  = 0;
	ret->cap_e = nb_elem;
	return (ret);
}
