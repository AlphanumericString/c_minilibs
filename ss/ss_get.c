#include <string.h>

#include "./sparse_set.h"

bool ss_set(t_ss *ss, size_t pos, void *elem) {
	if (!pos || pos > ss->nb_elem) return (false);
	memcpy(ss_at(ss, pos), elem, ss->sz_elem);
	return (true);
}
void *ss_at(t_ss *ss, size_t pos) {
	uint index;

	if (!ss || pos > ss->nb_elem) return (NULL);
	index = ss->indexes[pos];
	return (ss->data + index * ss->sz_elem);
}
void *ss_get(t_ss *ss, size_t pos, void *out) {
	if (!ss || pos > ss->nb_elem || !out) return (NULL);
	memcpy(out, ss_at(ss, pos), ss->sz_elem);
	return (out);
}
