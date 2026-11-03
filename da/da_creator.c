#include <stdlib.h>

#include "dynamic_array.h"

/**
 * \brief create a new da struct and allocate a buffer for elem of size sz_elem
 * \param sz_elem size of 1 element
 * \return pointer to the new dynamic array
 * \ingroup da
**/
t_da *da_new(size_t sz_elem) {
	t_da *ret;

	if (!sz_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return ((t_da *)NULL);
	}
	ret = (t_da *)calloc(sizeof(t_da), 1);
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC)
		return ((t_da *)NULL);
	}
	if (da_init(ret, sz_elem) == false) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC)
		free(ret);
		return ((t_da *)NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (ret);
}
/**
 * \brief init a given da with the default number of elements(1024) of size
 * sz_elem
 * \param ret pointer to the dynamic array struct to init
 * \param sz_elem size of 1 element
 * \return pointer to the initialized dynamic array
 * \ingroup da
**/
t_da *da_init(t_da *ret, size_t sz_elem) {
	void *data;

	if (!ret || !sz_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return ((t_da *)NULL);
	}
	data = (void *)calloc(sz_elem, C_MINI__DA_NB_ELEM_DFLT);
	if (!data) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC)
		return ((t_da *)NULL);
	}
	da_from_array(ret, data, C_MINI__DA_NB_ELEM_DFLT, sz_elem);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (ret);
}
/**
 * \brief init a given da with the number n of elements of size sz_elem
 * \param ret pointer to the dynamic array struct to init
 * \param n initial/starting capacity (number space for elements)
 * \param sz_elem size of 1 element
 * \return pointer to the initialized dynamic array
 * \ingroup da
**/
t_da *da_ninit(t_da *ret, size_t n, size_t sz_elem) {
	void *data;

	if (!ret || !sz_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return ((t_da *)NULL);
	}
	data = (void *)calloc(sz_elem, n);
	if (!data) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__DA_LOG_EALLOC)
		return ((t_da *)NULL);
	}
	da_from_array(ret, data, n, sz_elem);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (ret);
}
/**
 * \brief init a given da with the array given
 * \param ret pointer to the dynamic array struct to init
 * \param data pointer to the memory array
 * \param nb_elem the max number of elements the array can hold (capacity)
 * \param sz_elem the size of 1 element
 * \warning 'from_array' takes ownership so DO NOT USE NON-ALLOCATED array if
 * there may be a need for resizing. If you want a constant-sized array try
 * rb/round_buffers. If you want to copy values try da_cat_array.
 * \return pointer to the initialized dynamic array
 * \ingroup da
**/
t_da *da_from_array(t_da *ret, void *data, size_t nb_elem, size_t sz_elem) {
	if (!ret || !data || !sz_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return ((t_da *)NULL);
	}
	ret->array = data;
	ret->sz_e  = sz_elem;
	ret->nb_e  = 0;
	ret->cap_e = nb_elem;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (ret);
}
