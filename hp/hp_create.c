#include <stdlib.h>

#include "heap.h"

t_heap *hp_create(size_t nb_elems, t_comparator cmp) {
	t_heap *hp = malloc(sizeof(t_heap));

	if (!hp) {
		LOG(C_MINI__HP_LOG_EARG);
		return (NULL);
	}
	if (hp_init(hp, nb_elems, cmp) == NULL) {
		LOG(C_MINI__HP_LOG_EALLOC);
		return (free(hp), NULL);
	}
	LOG(C_MINI__HP_LOG_SUCCESS);
	return (hp);
}

t_heap *hp_init(t_heap *hp, size_t nb_elems, t_comparator cmp) {
	if (!hp) {
		LOG(C_MINI__HP_LOG_EARG);
		return (NULL);
	}
	hp->cmp      = cmp;
	hp->nb       = 0;
	hp->capacity = nb_elems;
	hp->elems    = calloc(hp->capacity, sizeof(void *));
	if (!hp->elems) {
		LOG(C_MINI__HP_LOG_EALLOC);
		return (NULL);
	}
	LOG(C_MINI__HP_LOG_SUCCESS);
	return (hp);
}
