#include <stdlib.h>

#include "dynamic_array.h"

/**
 * \brief clear - memory is reusable and only set 0
 * \param ret pointer to the dynamic array
 * \ingroup da
**/
void da_clr(t_da *ret) {
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG);
		return;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS);
	ret->nb_e = 0;
}
/**
 * \brief reset the da and free the memory in it
 * \param ret pointer to the dynamic array
 * \ingroup da
**/
void da_destroy(t_da *ret) {
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG);
		return;
	}
	if (ret->array) free(ret->array);
	*ret = (t_da){0};
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS);
}
/**
 * \brief destroy and free the da
 * \param da pointer to the dynamic array
 * \ingroup da
**/
void da_free(t_da *da) {
	if (!da) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG);
		return;
	}
	da_destroy(da);
	free(da);
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS);
}
