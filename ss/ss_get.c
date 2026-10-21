#include <string.h>

#include "sparse_set.h"

/**
 * \brief Try to set value elem into the array at pos. True if opperation
 * succeed
 * \param ss sparse set to set value in
 * \param pos position to set value in
 * \param elem value to set
 * \return true if opperation succeed
**/
bool ss_set(t_ss *ss, size_t pos, void *elem) {
	if (!pos || pos > ss->nb_elem) return (false);
	memcpy(ss_at(ss, pos), elem, ss->sz_elem);
	return (true);
}
/**
 * \brief Return offset into the array for index pos.
 * \param ss sparse set to get value from
 * \param pos position to get value from
 * \return offset into the array for index pos
**/
void *ss_at(t_ss *ss, size_t pos) {
	uint index;

	if (!ss || pos > ss->nb_elem) return (NULL);
	index = ss->indexes[pos];
	return (ss->data + index * ss->sz_elem);
}
/**
 * \brief Return offset into the array for index pos, and copy elem to out
 * pointer
 * \param ss sparse set to get value from
 * \param pos position to get value from
 * \param out pointer to copy value to
 * \return pointer to out (local)
**/
void *ss_get(t_ss *ss, size_t pos, void *out) {
	if (!ss || pos > ss->nb_elem || !out) return (NULL);
	memcpy(out, ss_at(ss, pos), ss->sz_elem);
	return (out);
}
