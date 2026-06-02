#include "./sparse_set.h"

void ss_remove(t_ss *ss, size_t pos) {
	if (pos > ss->nb_elem) return;
	if (pos == ss->nb_elem) return (ss->nb_elem--, (void)0);
	ss_swap(ss, pos, ss->nb_elem - 1);
	ss->indexes[pos] ^= ss->indexes[ss->nb_elem - 1];
	ss->indexes[ss->nb_elem - 1] ^= ss->indexes[pos];
	ss->indexes[pos] ^= ss->indexes[ss->nb_elem - 1];
	ss->nb_elem--;
	return;
}
void ss_nremove(t_ss *ss, size_t start, size_t end) {
	if (start > ss->nb_elem) return;
	if (end > ss->nb_elem)
		end = ss->nb_elem;
	if (end != ss->nb_elem) {
		// TODO: move stuff
	}
	ss->nb_elem -= (end - start);
	return;
}
