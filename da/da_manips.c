#include "./dynamic_array.h"

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

bool da_swap(t_da *vec, size_t pos_a, size_t pos_b) {
	unsigned char  sized_buffer[PVEC_SZ_ELEM_DFLT];
	unsigned char *sa;
	unsigned char *sb;

	if (!vec || vec->nb_e < pos_a || vec->nb_e < pos_b) return (false);
	sa = da_at(vec, pos_a);
	sb = da_at(vec, pos_b);
	if (!sa || !sb) return (false);
	if (vec->sz_e > PVEC_SZ_ELEM_DFLT) { // swap with xor
		memnxor(sa, sb, vec->sz_e);
		memnxor(sb, sa, vec->sz_e);
		memnxor(sa, sb, vec->sz_e);
	} else {
		memcpy(sized_buffer, sa, vec->sz_e);
		memcpy(sa, sb, vec->sz_e);
		memcpy(sb, sized_buffer, vec->sz_e);
	}
	return (true);
}
