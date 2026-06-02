#include "./dynamic_array.h"

void da_clr(t_da *ret) {
	if (!ret) return;
	ret->nb_e = 0;
}
void da_destroy(t_da *ret) {
	if (!ret) return;
	if (ret->array) free(ret->array);
	ret->sz_e  = 0;
	ret->array = NULL;
	ret->cap_e = 0;
	ret->nb_e  = 0;
}
