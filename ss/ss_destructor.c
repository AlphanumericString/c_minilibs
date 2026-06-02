#include "./sparse_set.h"

void ss_clr(t_ss *to_clear) {
	if (!to_clear) return;
	to_clear->nb_elem = 0;
}
void ss_destroy(t_ss *to_destroy) {
	if (!to_destroy) return;
	if (to_destroy->data) free(to_destroy->data);
	if (to_destroy->indexes) free(to_destroy->indexes);
	*to_destroy = (t_ss){0};
}
