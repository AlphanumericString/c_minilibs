#include <stdlib.h>

#include "heap.h"

/**
 * \brief Create a heap that takes ownership of a pre-existing memory buffer
 * \param arr raw buffer used as the heap's backing storage (no element is
 * initialized by this function, the heap starts empty)
 * \param nb_elems max number of elements the buffer can hold (capacity)
 * \param sz_elem size of one element
 * \param cmp comparator function
 * \warning 'from_array' takes ownership of 'arr', do not use a non-allocated
 * or still referenced array. The buffer is used as-is: this function does not
 * copy or heapify its contents. Use hp_{min,max}_{new,init}_from_array to
 * build a heap from already initialized elements.
 * \return t_cm_hp* or NULL if failed
 * \ingroup hp
**/
t_cm_hp *hp_new_from_array(void *arr, size_t nb_elems, size_t sz_elem,
                           t_cm_comparator cmp) {
	t_cm_hp *hp;

	hp = malloc(sizeof(t_cm_hp));
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__HP_LOG_EALLOC)
		return (NULL);
	}
	if (hp_init_from_array(hp, arr, nb_elems, sz_elem, cmp) == NULL) {
		free(hp);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp);
}

/**
 * \brief Init a heap from a pre-existing memory buffer
 * \param hp heap pointer
 * \param arr raw buffer used as the heap's backing storage (no element is
 * initialized by this function, the heap starts empty)
 * \param nb_elems max number of elements the buffer can hold (capacity)
 * \param sz_elem size of one element
 * \param cmp comparator function (elem is always on the left, arg on the right)
 * \warning 'from_array' takes ownership of 'arr', do not use a non-allocated
 * or still referenced array. The buffer is used as-is: this function does not
 * copy or heapify its contents. Use hp_{min,max}_{new,init}_from_array to
 * build a heap from already initialized elements.
 * \return t_cm_hp* or NULL if failed
 * \ingroup hp
**/
t_cm_hp *hp_init_from_array(t_cm_hp *hp, void *arr, size_t nb_elems,
                            size_t sz_elem, t_cm_comparator cmp) {
	if (!hp || !arr || !nb_elems || !sz_elem || !cmp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	hp->cmp      = cmp;
	hp->nb       = 0;
	hp->capacity = nb_elems;
	hp->sz_e     = sz_elem;
	hp->elems    = arr;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp);
}
