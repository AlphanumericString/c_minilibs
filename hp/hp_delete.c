#include <stdlib.h>

#include "heap.h"

/**
 * \brief Delete a heap internal structure
 * \param hp heap pointer
 * \ingroup hp
**/
bool hp_delete(t_hp *hp) {
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	if (hp->elems) free(hp->elems);
	*hp = (t_hp){0};
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}

/**
 * \brief Delete a heap and free its memory (internal and external)
 * \param hp heap pointer
 * \ingroup hp
**/
bool hp_free(t_hp **hp) {
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	if (*hp) {
		hp_delete(*hp);
		free(*hp);
		*hp = NULL;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_SUCCESS)
	return (true);
}

/**
 * \brief Clear a heap
 * \param hp heap pointer
 * \ingroup hp
**/
bool hp_clear(t_hp *hp) {
	if (!hp) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__HP_LOG_EARG)
		return (false);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_CLR)
	hp->nb = 0;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__HP_LOG_STATUS, (void *)hp, hp->nb,
	       hp->capacity, hp->sz_e)
	return (true);
}
