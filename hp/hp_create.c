#include <stdlib.h>

#include "heap.h"

/**
 * \brief Create and init a heap
 * \param nb_elems max number of elements
 * \param sz_elem size of one element
 * \param cmp comparator function
 * \return t_hp* or NULL if failed
 * \ingroup hp
**/
t_hp *hp_create(size_t nb_elems, size_t sz_elem, t_cm_comparator cmp) {
	t_hp *hp;

	hp = malloc(sizeof(t_hp));
	if (!hp || hp_init(hp, nb_elems, sz_elem, cmp) == NULL) {
		if (hp) free(hp);
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EALLOC)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp);
}

/**
 * \brief Init a heap
 * \param hp heap pointer
 * \param nb_elems max number of elements
 * \param sz_elem size of one element
 * \param cmp comparator function (elem is always on the left, arg on the right)
 * \return t_hp* or NULL if failed
 * \ingroup hp
**/
t_hp *hp_init(t_hp *hp, size_t nb_elems, size_t sz_elem, t_cm_comparator cmp) {
	if (!hp || !sz_elem) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (NULL);
	}
	hp->cmp      = cmp;
	hp->nb       = 0;
	hp->capacity = nb_elems;
	hp->sz_e     = sz_elem;
	hp->elems    = calloc(nb_elems, sz_elem);
	if (!hp->elems) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__HP_LOG_EALLOC)
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (hp);
}
