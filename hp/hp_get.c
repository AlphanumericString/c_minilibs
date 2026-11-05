#include "heap.h"

/**
 * \brief get the comparator of the heap
 * \param hp pointer of the heap
 * \return the comparator or NULL on failure
 * \ingroup hp
**/
t_cm_comparator hp_get_cmp(const t_cm_hp *hp) {
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp->cmp);
}
/**
 * \brief get the capacity of the heap
 * \param hp pointer of the heap
 * \return the capacity or 0 on failure
 * \ingroup hp
**/
size_t hp_get_cap(const t_cm_hp *hp) {
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp->capacity);
}
/**
 * \brief get the number of elements currently in the heap
 * \param hp pointer of the heap
 * \return the number of elements or 0 on failure
 * \ingroup hp
**/
size_t hp_get_current_size(const t_cm_hp *hp) {
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp->nb);
}
/**
 * \brief get the size in bytes of one element of the heap
 * \param hp pointer of the heap
 * \return the size of one element or 0 on failure
 * \ingroup hp
**/
size_t hp_get_elem_size(const t_cm_hp *hp) {
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp->sz_e);
}
/**
 * \brief get the raw inner byte buffer of the heap
 * \param hp pointer of the heap
 * \return pointer to the raw inner array or NULL on failure
 * \ingroup hp
**/
void *hp_get_elems_array(const t_cm_hp *hp) {
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp->elems);
}
/**
 * \brief get a pointer to the element at offset pos respecting the size of
 * elements
 * \param hp pointer of the heap
 * \param pos index of the element
 * \return pointer to the element at the specified position or NULL on failure
 * \ingroup hp
**/
void *hp_at(const t_cm_hp *hp, size_t pos) {
	if (!hp || !hp->elems || !hp->sz_e || pos >= hp->nb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return ((char *)hp->elems + hp->sz_e * pos);
}
