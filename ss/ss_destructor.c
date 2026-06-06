#include "sparse_set.h"

/**
 * \brief Clear ss. Does not free sub arrays.
 * \param to_clear ss to clear
**/
void ss_clr(t_ss *to_clear) {
	if (!to_clear) return;
	to_clear->nb_elem = 0;
}
/**
 * \brief Free's sub arrays and set all fields to NULL values. DOES NOT FREE SS.
 * \param to_destroy ss to destroy sub arrays of
**/
void ss_destroy(t_ss *to_destroy) {
	if (!to_destroy) return;
	if (to_destroy->data) free(to_destroy->data);
	if (to_destroy->indexes) free(to_destroy->indexes);
	*to_destroy = (t_ss){0};
}
