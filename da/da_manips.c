#include "dynamic_array.h"

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
 * \brief swap the values in the da at 2 offsets
 * \param vec pointer to the dynamic array
 * \param pos_a index of the first element
 * \param pos_b index of the second element
 * \return true on success, false on failure
 **/
bool da_swap(t_da *da, size_t pos_a, size_t pos_b) {
	unsigned char  sized_buffer[C_MINI_DA__SZ_ELEM_DFLT];
	unsigned char *sa;
	unsigned char *sb;

	if (!da || da->nb_e < pos_a || da->nb_e < pos_b) return (false);
	sa = da_at(da, pos_a);
	sb = da_at(da, pos_b);
	if (!sa || !sb) return (false);
	if (da->sz_e > C_MINI_DA__SZ_ELEM_DFLT) { // swap with xor
		memnxor(sa, sb, da->sz_e);
		memnxor(sb, sa, da->sz_e);
		memnxor(sa, sb, da->sz_e);
	} else {
		memcpy(sized_buffer, sa, da->sz_e);
		memcpy(sa, sb, da->sz_e);
		memcpy(sb, sized_buffer, da->sz_e);
	}
	return (true);
}

/**
 * \brief swap n elements between two positions in the dynamic array
 * \param da pointer to the dynamic array
 * \param n number of elements to swap
 * \param pos_a index of the first position
 * \param pos_b index of the second position
 * \return true on success, false otherwise
 **/
bool da_nswap(t_da *da, size_t n, size_t pos_a, size_t pos_b) {
	unsigned char  sized_buffer[C_MINI_DA__SZ_ELEM_DFLT];
	unsigned char *sa;
	unsigned char *sb;

	if (!da || da->nb_e < (pos_a + n) || da->nb_e < (pos_b + n)) return (false);
	if (!n) return (true);
	sa = da_at(da, pos_a);
	sb = da_at(da, pos_b);
	if (!sa || !sb) return (false);
	if (da->sz_e * n > C_MINI_DA__SZ_ELEM_DFLT) { // swap with xor
		memnxor(sa, sb, da->sz_e * n);
		memnxor(sb, sa, da->sz_e * n);
		memnxor(sa, sb, da->sz_e * n);
	} else {
		memcpy(sized_buffer, sa, da->sz_e * n);
		memcpy(sa, sb, da->sz_e * n);
		memcpy(sb, sized_buffer, da->sz_e * n);
	}
	return (true);
}
