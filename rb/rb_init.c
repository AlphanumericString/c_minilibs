#include "round_buffer.h"

/**
 * \brief populate rb's field and allocate array from sz_e and nb_e
 * \param rb the round buffer to populate
 * \param sz_elem size of each element
 * \param nb_e number of elements
 * \return the populated round buffer or NULL on failure
**/
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
/**
 * \brief populate rb's field using the default nb_e and given sz_e. allocate
 * array
 * \param rb the round buffer to populate
 * \param sz_elem size of each element
 * \return the populated round buffer or NULL on failure
**/
t_rb *rb_init(t_rb *rb, size_t sz_elem) {
	return (rb_create(rb, sz_elem, C_MINI_RB_NB_ELEM_DFLT));
}
/**
 * \brief populate rb from given values, does not allocate array
 * \param rb the round buffer to populate
 * \param array the pre-allocated array to use
 * \param nb_elem number of elements the array can hold
 * \param sz_elem size of each element
 * \return the populated round buffer
**/
t_rb *rb_from_array(t_rb *rb, void *array, size_t nb_elem, size_t sz_elem) {
	rb->i_pos  = 0;
	rb->o_pos  = 0;
	rb->sz_e   = sz_elem;
	rb->nb_max = nb_elem;
	rb->array  = array;
	return (rb);
}
/**
 * \brief allocate a new rb and populate it with default nb_e and given sz_e
 * \param sz_elem size of each element
 * \return the newly allocated round buffer or NULL on failure
**/
t_rb *rb_new(size_t sz_elem) {
	t_rb *ret;
	if (!sz_elem) return (NULL);
	ret = malloc(sizeof(t_rb));
	if (rb_init(ret, sz_elem) == NULL) return (free(ret), NULL);
	return (ret);
}
