#include "./round_buffer.h"

t_rb *rb_create(t_rb *rb, size_t sz_elem, size_t nb_e) {
	if (!rb || !sz_elem || !nb_e) return (NULL);
	rb->array = malloc(sz_elem * nb_e);
	if (!rb->array) return (NULL);
	rb->sz_e   = sz_elem;
	rb->nb_max = nb_e;
	rb->o_pos  = 0;
	rb->i_pos  = 0;
	return (rb);

}
t_rb *rb_init(t_rb *rb, size_t sz_elem) {
	return (rb_create(rb, sz_elem, PRB_NB_ELEM_DFLT));
}
t_rb *rb_from_array(t_rb *rb, void *array, size_t nb_elem, size_t sz_elem) {
	rb->i_pos  = 0;
	rb->o_pos  = 0;
	rb->sz_e   = sz_elem;
	rb->nb_max = nb_elem;
	rb->array  = array;
	return (rb);
}
t_rb *rb_new(size_t sz_elem) {
	t_rb *ret;
	if (!sz_elem) return (NULL);
	ret = malloc(sizeof(t_rb));
	if (rb_init(ret, sz_elem) == NULL) return (free(ret), NULL);
	return (ret);
}
