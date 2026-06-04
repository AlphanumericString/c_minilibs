#include <string.h>

#include "./sparse_set.h"

static inline void memnxor(void *a, void *b, size_t n) {
	unsigned char *s_a;
	unsigned char *s_b;

	if (!n || !a || !b) return;
	s_a = a;
	s_b = b;
	while (n--)
		s_a[n] ^= s_b[n];
	return;
}
/**
 * \brief Swap values at index pos_a and pos_b together.
 * \param ss sparse set to swap values in
 * \param pos_a position to swap value from
 * \param pos_b position to swap value to
 * \return true if opperation succeed
**/
bool ss_swap(t_ss *ss, size_t pos_a, size_t pos_b) {
	unsigned char  sized_buffer[4096];
	unsigned char *sa;
	unsigned char *sb;
	unsigned int   tmp;

	if (!ss || ss->nb_elem < pos_a || ss->nb_elem < pos_b) return (false);
	sa = ss_at(ss, pos_a);
	sb = ss_at(ss, pos_b);
	if (!sa || !sb) return (false);
	if (ss->sz_elem > 4096) { // swap with xor
		memnxor(sa, sb, ss->sz_elem);
		memnxor(sb, sa, ss->sz_elem);
		memnxor(sa, sb, ss->sz_elem);
	} else { // swap with buffer
		memcpy(sized_buffer, sa, ss->sz_elem);
		memcpy(sa, sb, ss->sz_elem);
		memcpy(sb, sized_buffer, ss->sz_elem);
	}
	tmp                = ss->indexes[pos_a];
	ss->indexes[pos_a] = ss->indexes[pos_b];
	ss->indexes[pos_b] = tmp;
	return (true);
}
