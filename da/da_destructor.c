#include <stdlib.h>

#include "dynamic_array.h"

/**
 * \brief clear - memory is reusable and only set 0
 * \param ret pointer to the dynamic array
**/
void da_clr(t_da *ret) {
	if (!ret) return;
	ret->nb_e = 0;
}
/**
 * \brief destroy the da and the memory in it
 * \param ret pointer to the dynamic array
**/
void da_destroy(t_da *ret) {
	if (!ret) return;
	if (ret->array) free(ret->array);
	ret->sz_e  = 0;
	ret->array = NULL;
	ret->cap_e = 0;
	ret->nb_e  = 0;
}
