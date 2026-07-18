#include <stdlib.h>

#include "heap.h"

/**
 * \brief Delete a heap internal structure
 * \param hp heap pointer
 * \ingroup hp
 **/
void hp_delete(t_heap *hp) {
	if (!hp) {
		LOG(C_MINI__HP_LOG_EARG);
		return;
	}
	if (hp->elems) free(hp->elems);
	*hp = (t_heap){0};
	LOG(C_MINI__HP_LOG_SUCCESS);
}

/**
 * \brief Delete a heap and free its memory (internal and external)
 * \param hp heap pointer
 * \ingroup hp
 **/
void hp_free(t_heap *hp) {
	if (!hp) {
		LOG(C_MINI__HP_LOG_EARG);
		return;
	}
	hp_delete(hp);
	free(hp);
	LOG(C_MINI__HP_LOG_SUCCESS);
}

/**
 * \brief Clear a heap
 * \param hp heap pointer
 * \ingroup hp
 **/
void hp_clear(t_heap *hp) {
	if (!hp) {
		LOG(C_MINI__HP_LOG_EARG);
		return;
	}
	LOG(C_MINI__HP_LOG_CLR);
	hp->nb = 0;
	LOG(C_MINI__HP_LOG_STATUS, hp, hp->nb, hp->capacity);
}
