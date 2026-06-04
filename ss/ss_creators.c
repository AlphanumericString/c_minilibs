#include "./sparse_set.h"

/**
 * \brief Allocates and init a new ss for elements of size sz_elem
 * \param sz_elem size of elements
 * \return new ss
**/
t_ss *ss_new(size_t sz_elem) {
	return (ss_init((t_ss *)calloc(sizeof(t_ss), 1), sz_elem));
}
/**
 * \brief Init a given ss for elements of size sz_elem. Allocates sub arrays
 * \param ss ss to init
 * \param sz_elem size of elements
 * \return ss
**/
t_ss *ss_init(t_ss *ss, size_t sz_elem) {
	if (!ss || !sz_elem) return (NULL);
	return (ss_ninit(ss, PSS_NB_ELEM_DFLT, sz_elem));
}
/**
 * \brief Init a given ss for n elements of size sz_elem. Allocates sub arrays
 * \param ss ss to init
 * \param nb_elem number of elements
 * \param sz_elem size of elements
 * \return return a pointer to the initiated ss pointer
**/
t_ss *ss_ninit(t_ss *ss, size_t nb_elem, size_t sz_elem) {
	void *data;
	uint *index;

	if (!ss || !nb_elem || !sz_elem) return (NULL);
	data  = calloc(sz_elem, nb_elem);
	index = malloc(sizeof(*index) * nb_elem);
	return (ss_from_array(ss, data, index, nb_elem, sz_elem));
}
/**
 * \brief Init a given ss with sub array given and nb_elem + sz_elem
 * \param ss ss to init
 * \param array array to init with
 * \param index_array index array to init with
 * \param nb_elem number of elements
 * \param sz_elem size of elements
 * \return ss
 * \warning from array takes ownership of said array.
 * futhermore the array is free'd if theres any need for resizing.
 * if you want a constant-sized array try rb/round_buffers.
 * if you want to copy values try ss_cat_array
**/
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
