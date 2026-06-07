#include "dynamic_array.h"

t_da da_dup(const t_da *src) {
	return (da_ndup(src, -1));
}

t_da da_ndup(const t_da *src, size_t n) {
	t_da ret = {.array = NULL, .nb_e = 0, .cap_e = 0, .sz_e = 0};

	if (!src || !n)
		return (ret);
	if (n > src->nb_e)
		n = src->nb_e;
	if (!da_ninit(&ret, n, src->sz_e))
		return (ret);
	ret.nb_e = n;
	return (memcpy(ret.array, src->array, n * src->sz_e), ret);
}

t_da *da_dup_to(t_da *dst, const t_da *src) {
	return (da_ndup_to(dst, -1, src));
}

t_da *da_ndup_to(t_da *dst, size_t n, const t_da *src) {
	void *pos;

	if (!src || !dst || !n)
		return (dst);
	if (n > src->nb_e)
		n = src->nb_e;
	if (dst->cap_e - dst->nb_e < n)
		da_grow_for_size(dst, n);
	dst->nb_e += n;
	pos = da_at(dst, dst->nb_e - n);
	return (memcpy(pos, src->array, n * src->sz_e), dst);
}
