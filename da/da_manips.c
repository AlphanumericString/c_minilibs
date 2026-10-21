#include <string.h>

#include "dynamic_array.h"

static inline void memnxor(void *a, void *b, size_t n) {
	unsigned char *s_a = (unsigned char *)a;
	unsigned char *s_b = (unsigned char *)b;

	while (n--)
		s_a[n] ^= s_b[n];
}

/**
 * \brief swap the values in the da at 2 offsets
 * \param da pointer to the dynamic array
 * \param pos_a index of the first element
 * \param pos_b index of the second element
 * \return true on success, false on failure
 * \ingroup da
**/
bool da_swap(t_da *da, size_t pos_a, size_t pos_b) {
	unsigned char  sized_buffer[C_MINI_DA__SZ_ELEM_DFLT];
	unsigned char *sa;
	unsigned char *sb;

	sa = (unsigned char *)da_at(da, pos_a);
	sb = (unsigned char *)da_at(da, pos_b);
	if (!sa || !sb) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if (da->sz_e > C_MINI_DA__SZ_ELEM_DFLT) {
		memnxor(sa, sb, da->sz_e);
		memnxor(sb, sa, da->sz_e);
		memnxor(sa, sb, da->sz_e);
	} else {
		memcpy(sized_buffer, sa, da->sz_e);
		memcpy(sa, sb, da->sz_e);
		memcpy(sb, sized_buffer, da->sz_e);
	}
	//	CM_LOG(C_MINI__DA_LOG_SWAP, 1UL, pos_a, pos_b)
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}
/**
 * \brief swap n elements between two positions in the dynamic array
 * \param da pointer to the dynamic array
 * \param n number of elements to swap
 * \param pos_a index of the first position
 * \param pos_b index of the second position
 * \return true on success, false otherwise
 * \ingroup da
**/
bool da_nswap(t_da *da, size_t n, size_t pos_a, size_t pos_b) {
	unsigned char  sized_buffer[C_MINI_DA__SZ_ELEM_DFLT];
	unsigned char *sa;
	unsigned char *sb;

	if (!da_is_valid(da) || da->nb_e < (pos_a + n) || da->nb_e < (pos_b + n)) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__DA_LOG_EARG)
		return (false);
	}
	if (!n) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SWAP, n, pos_a, pos_b)
		return (true);
	}
	sa = (unsigned char *)da_at(da, pos_a);
	sb = (unsigned char *)da_at(da, pos_b);
	if (da->sz_e * n > C_MINI_DA__SZ_ELEM_DFLT) {
		memnxor(sa, sb, da->sz_e * n);
		memnxor(sb, sa, da->sz_e * n);
		memnxor(sa, sb, da->sz_e * n);
	} else {
		memcpy(sized_buffer, sa, da->sz_e * n);
		memcpy(sa, sb, da->sz_e * n);
		memcpy(sb, sized_buffer, da->sz_e * n);
	}
	// CM_LOG(C_MINI__DA_LOG_SWAP, n, pos_a, pos_b)
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__DA_LOG_SUCCESS)
	return (true);
}
