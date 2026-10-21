#include "sparse_set.h"

/**
 * \brief Remove value at pos.
 * \param ss ss to remove value from
 * \param pos position to remove value from
**/
void ss_remove(t_ss *ss, size_t pos) {
	if (pos > ss->nb_elem) return;
	if (pos == ss->nb_elem) {
		ss->nb_elem--;
		return;
	}
	ss_swap(ss, pos, ss->nb_elem - 1);
	ss->indexes[pos]             ^= ss->indexes[ss->nb_elem - 1];
	ss->indexes[ss->nb_elem - 1] ^= ss->indexes[pos];
	ss->indexes[pos]             ^= ss->indexes[ss->nb_elem - 1];
	ss->nb_elem--;
	return;
}
/**
 * \brief Remove value from start to end (excluding end [start, end[ ).
 * \param ss ss to remove value from
 * \param start start position to remove value from
 * \param end end position to remove value from (end excluded)
 * \warning not fully implemented.
 * \warning if start superior to number of elements silently fails
**/
void ss_nremove(t_ss *ss, size_t start, size_t end) {
	if (start > ss->nb_elem) return;
	if (end > ss->nb_elem) end = ss->nb_elem;
	if (end != ss->nb_elem) {
		// TODO: move stuff
	}
	ss->nb_elem -= (end - start);
	return;
}
