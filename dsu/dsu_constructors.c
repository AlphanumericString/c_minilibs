#include <string.h>

#include "disjointed_set_union.h"

/**
 * \brief init by allocating both the return and the internals
 * \param n size of the dsu
 * \return the new t_dsu
**/
t_dsu *dsu_new(size_t n) {
	t_dsu *ret;
	void  *buffer;

	if (!n) return (NULL);
	ret = malloc(sizeof(*ret));
	if (!ret) return (NULL);
	*ret   = (t_dsu){0};
	buffer = malloc(sizeof(int) * n * 2);
	if (!buffer) return (free(ret), NULL);
	dsu_init_from_arrays(ret, n, buffer, buffer + n * sizeof(int));
	ret->rp_single_alloc = true;
	return (ret);
}
/**
 * \brief init by allocating the internals
 * \param to_init the t_dsu to init
 * \param n size of the dsu
 * \return the initialized t_dsu
**/
t_dsu *dsu_init(t_dsu *to_init, size_t n) {
	void *buffer;

	if (!n || !to_init) return (NULL);
	buffer = calloc(sizeof(int), n * 2);
	if (!buffer) return (NULL);
	dsu_init_from_arrays(to_init, n, buffer, buffer + n * sizeof(int));
	to_init->rp_single_alloc = true;
	return (to_init);
}
/**
 * \brief init by allocating a t_dsu and using provided values
 * \param size size of the dsu
 * \param parents the parents array
 * \param ranks the ranks array
 * \return the new t_dsu
**/
t_dsu *dsu_new_from_array(size_t size, int *parents, int *ranks) {
	t_dsu *ret;

	if (!size || !parents || !ranks) return (NULL);
	ret = malloc(sizeof(t_dsu));
	if (!ret) return (NULL);
	ret->rank            = (uint *)ranks;
	ret->parent          = (uint *)parents;
	ret->size            = size;
	ret->rp_single_alloc = false;
	dsu_clr(ret);
	return (ret);
}
/**
 * \brief init t_dsu using provided values (no allocs)
 * \param ret the t_dsu to init
 * \param size size of the dsu
 * \param parents the parents array
 * \param ranks the ranks array
 * \return the initialized t_dsu
**/
t_dsu *dsu_init_from_arrays(t_dsu *ret, size_t size, int *parents, int *ranks) {
	if (!ret || !size || !parents || !ranks) return (NULL);
	ret->rank            = (uint *)ranks;
	ret->parent          = (uint *)parents;
	ret->size            = size;
	ret->components      = size;
	ret->rp_single_alloc = false;
	dsu_clr(ret);
	return (ret);
}
/**
 * \brief grow the dsu by N values
 *
 * (warning: will realocate. if you used local buffers durring construction
 * via from_arrays methods do not use).
 * \param dsu the t_dsu to grow
 * \param n the number of values to add
 * \return the grown t_dsu
**/
t_dsu *dsu_grow(t_dsu *dsu, size_t n) {
	void  *replacement_buffer;
	void  *rank_offset;
	size_t i;

	if (!dsu || !n) return (dsu);
	if (!dsu->size || !dsu->rank || !dsu->parent) return (dsu);
	replacement_buffer = malloc(sizeof(int) * (dsu->size + n) * 2);
	if (!replacement_buffer) return (dsu);
	rank_offset = replacement_buffer + (dsu->size + n) * sizeof(int);
	memcpy(replacement_buffer, dsu->parent, dsu->size * sizeof(int));
	memcpy(rank_offset, dsu->rank, dsu->size * sizeof(int));
	free(dsu->parent);
	dsu->parent = replacement_buffer;
	if (!dsu->rp_single_alloc) free(dsu->rank);
	dsu->rank            = rank_offset;
	dsu->rp_single_alloc = true;
	i                    = 0;
	while (i < n) {
		dsu->parent[i + dsu->size] = i + dsu->size;
		dsu->rank[i++ + dsu->size] = 0;
	}
	dsu->size       += n;
	dsu->components += n;
	return (dsu);
}
