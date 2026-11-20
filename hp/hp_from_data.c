#include <stdlib.h>

#include "heap.h"

/**
 * \brief Create a min heap from a buffer already filled with nb_elems
 * initialized elements
 * \param arr raw buffer containing nb_elems initialized elements
 * \param nb_elems number of initialized elements in arr (and heap capacity)
 * \param sz_elem size of one element
 * \param cmp comparator function
 * \warning this constructor takes ownership of arr, do not use a non-allocated
 * or still referenced array. On failure arr is left to the caller.
 * \return t_cm_hp* or NULL if failed
 * \ingroup hp
**/
t_cm_hp *hp_min_new_from_array(void *arr, size_t nb_elems, size_t sz_elem,
                               t_cm_comparator cmp) {
	t_cm_hp *hp;

	hp = malloc(sizeof(t_cm_hp));
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__HP_LOG_EALLOC)
		return (NULL);
	}
	if (!hp_min_init_from_array(hp, arr, nb_elems, sz_elem, cmp)) {
		free(hp);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp);
}

/**
 * \brief Create a max heap from a buffer already filled with nb_elems
 * initialized elements
 * \param arr raw buffer containing nb_elems initialized elements
 * \param nb_elems number of initialized elements in arr (and heap capacity)
 * \param sz_elem size of one element
 * \param cmp comparator function
 * \warning this constructor takes ownership of arr, do not use a non-allocated
 * or still referenced array. On failure arr is left to the caller.
 * \return t_cm_hp* or NULL if failed
 * \ingroup hp
**/
t_cm_hp *hp_max_new_from_array(void *arr, size_t nb_elems, size_t sz_elem,
                               t_cm_comparator cmp) {
	t_cm_hp *hp;

	hp = malloc(sizeof(t_cm_hp));
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__HP_LOG_EALLOC)
		return (NULL);
	}
	if (!hp_max_init_from_array(hp, arr, nb_elems, sz_elem, cmp)) {
		free(hp);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp);
}

/**
 * \brief Initialize a min heap from a buffer already filled with nb_elems
 * initialized elements
 * \param hp heap pointer
 * \param arr raw buffer containing nb_elems initialized elements
 * \param nb_elems number of initialized elements in arr (and heap capacity)
 * \param sz_elem size of one element
 * \param cmp comparator function
 * \warning this constructor takes ownership of arr, do not use a non-allocated
 * or still referenced array. On failure arr is left to the caller.
 * \return t_cm_hp* or NULL if failed
 * \ingroup hp
**/
t_cm_hp *hp_min_init_from_array(t_cm_hp *hp, void *arr, size_t nb_elems,
                                size_t sz_elem, t_cm_comparator cmp) {
	if (!hp || !arr || !nb_elems || !sz_elem || !cmp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	hp->cmp      = cmp;
	hp->nb       = nb_elems;
	hp->capacity = nb_elems;
	hp->sz_e     = sz_elem;
	hp->elems    = arr;
	hp_min_heapify(hp);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp);
}

/**
 * \brief Initialize a max heap from a buffer already filled with nb_elems
 * initialized elements
 * \param hp heap pointer
 * \param arr raw buffer containing nb_elems initialized elements
 * \param nb_elems number of initialized elements in arr (and heap capacity)
 * \param sz_elem size of one element
 * \param cmp comparator function
 * \warning this constructor takes ownership of arr, do not use a non-allocated
 * or still referenced array. On failure arr is left to the caller.
 * \return t_cm_hp* or NULL if failed
 * \ingroup hp
**/
t_cm_hp *hp_max_init_from_array(t_cm_hp *hp, void *arr, size_t nb_elems,
                                size_t sz_elem, t_cm_comparator cmp) {
	if (!hp || !arr || !nb_elems || !sz_elem || !cmp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	hp->cmp      = cmp;
	hp->nb       = nb_elems;
	hp->capacity = nb_elems;
	hp->sz_e     = sz_elem;
	hp->elems    = arr;
	hp_max_heapify(hp);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp);
}
