#include <stdlib.h>

#include "heap.h"

/**
 * \brief Create and init a heap
 * \param nb_elems max number of elements
 * \param cmp comparator function
 * \return t_heap* or NULL if failed
 * \ingroup hp
**/
t_heap *hp_create(size_t nb_elems, t_comparator cmp) {
	t_heap *hp;

	hp = malloc(sizeof(t_heap));
	if (!hp || hp_init(hp, nb_elems, cmp) == NULL) {
		if (hp) free(hp);
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EALLOC);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS);
	return (hp);
}

/**
 * \brief Init a heap
 * \param hp heap pointer
 * \param nb_elems max number of elements
 * \param cmp comparator function
 * \return t_heap* or NULL if failed
 * \ingroup hp
**/
t_heap *hp_init(t_heap *hp, size_t nb_elems, t_comparator cmp) {
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG);
		return (NULL);
	}
	hp->cmp      = cmp;
	hp->nb       = 0;
	hp->capacity = nb_elems;
	hp->elems    = calloc(hp->capacity, sizeof(void *));
	if (!hp->elems) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__HP_LOG_EALLOC);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS);
	return (hp);
}
