#include <stdlib.h>

#include "heap.h"

/**
 * \brief Delete a heap internal structure
 * \param hp heap pointer
 * \ingroup hp
**/
void hp_delete(t_heap *hp) {
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return;
	}
	if (hp->elems) free(hp->elems);
	*hp = (t_heap){0};
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
}

/**
 * \brief Delete a heap and free its memory (internal and external)
 * \param hp heap pointer
 * \ingroup hp
**/
void hp_free(t_heap *hp) {
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return;
	}
	hp_delete(hp);
	free(hp);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
}

/**
 * \brief Clear a heap
 * \param hp heap pointer
 * \ingroup hp
**/
void hp_clear(t_heap *hp) {
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_CLR)
	hp->nb = 0;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_STATUS, (void *)hp, hp->nb,
	       hp->capacity)
}
