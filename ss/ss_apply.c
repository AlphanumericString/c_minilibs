#include "sparse_set.h"

/**
 * \brief Applies func to all elements (un-ordered). True if opperation succeed.
 * \param ss ss to apply func to
 * \param func function to apply to all elements
 * \return true if opperation succeed
**/
bool ss_apply(t_ss *ss, void (*func)(void *elem)) {
	size_t i;

	if (!ss || !func) return (false);
	for (i = 0; i < ss->nb_elem; i++)
		func(ss_at(ss, i));
	return (true);
}
/**
 * \brief Applies func to all elements (un-ordered). True if opperation succeed.
 * \param ss ss to apply func to
 * \param func function to apply to all elements
 * \return true if opperation succeed
**/
bool ss_apply_it(t_ss *ss, void (*func)(void *elem, size_t pos, bool last)) {
	size_t i;

	if (!ss || !func) return (false);
	for (i = 0; i < ss->nb_elem; i++)
		func(ss_at(ss, i), i, (i + 1) == ss->nb_elem);
	return (true);
}
/**
 * \brief Applies func to all elements with u_val in order. Returns u_val.
 * \param ss ss to apply func to
 * \param func function to apply to all elements
 * \param u_val value to apply func to
 * \return u_val
**/
void *ss_fold(t_ss *ss, void *u_val, void (*func)(void *u_val, void *e_val)) {
	if (!ss || !func) return (NULL);
	for (size_t i = 0; i < ss->nb_elem; i++)
		func(u_val, ss_at(ss, i));
	return (u_val);
}
