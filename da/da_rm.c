#include "./dynamic_array.h"

// BUG: bug on the swap
// need to swap smallest between to_delete and right_gap
// rn if you try to delete something begger than right_gap shits starts to break

bool da_rm_order(t_da *vec, size_t pos) {
	if (!vec) return (false);
	if (vec->nb_e < pos) return (false);
	if (vec->nb_e != pos)
		memcpy(da_at(vec, pos), da_at(vec, pos + 1),
		       vec->sz_e * (vec->nb_e - pos));
	vec->nb_e--;
	return (true);
}
bool da_rms_order(t_da *vec, size_t start, size_t end) {
	if (end < start) return (false);
	if (!vec || vec->nb_e < start) return (false);
	if (vec->nb_e < end) end = vec->nb_e;
	if (vec->nb_e != end) {
		memcpy(da_at(vec, start), da_at(vec, end),
		       vec->sz_e * (vec->nb_e - end));
	}
	vec->nb_e -= end - start;
	return (false);
}

bool da_rm(t_da *vec, size_t pos) {
	if (!vec || vec->nb_e < pos) return (false);
	if (pos != vec->nb_e) da_swap(vec, pos, vec->nb_e - 1);
	vec->nb_e--;
	return (true);
}
bool da_rms(t_da *vec, size_t start, size_t end) {
	if (end < start) return (false);
	if (!vec || vec->nb_e < start) return (false);
	if (vec->nb_e < end) end = vec->nb_e;
	if (vec->nb_e != end) {
		memcpy(da_at(vec, start), da_at(vec, vec->nb_e - (end - start)),
		       vec->sz_e * (end - start));
	}
	vec->nb_e -= end - start;
	return (true);
}
