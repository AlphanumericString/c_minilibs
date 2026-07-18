#include <stdlib.h>
#include <string.h>

#include "compact_disjointed_set_union.h"

/**
 * \brief init by allocating both the return and the internals
 * \param n size of the cdsu
 * \return the new t_cdsu
 * \ingroup cdsu
**/
t_cdsu *cdsu_new(size_t n) {
	t_cdsu *ret;
	void   *buffer;

	if (!n) {
		LOG(C_MINI__CDSU_LOG_EARG);
		return (NULL);
	}
	ret = malloc(sizeof(*ret));
	if (!ret) {
		LOG(C_MINI__CDSU_LOG_EALLOC);
		return (NULL);
	}
	*ret   = (t_cdsu){0};
	buffer = malloc(sizeof(int) * n * 2);
	if (!buffer) {
		free(ret);
		LOG(C_MINI__CDSU_LOG_EALLOC);
		return (NULL);
	}
	cdsu_init_from_arrays(ret, n, buffer, buffer + n * sizeof(int));
	ret->rp_single_alloc = true;
	LOG("New DSU (cdsu:%p, size:%zu)", ret, n);
	return (ret);
}
/**
 * \brief init by allocating the internals
 * \param to_init the t_cdsu to init
 * \param n size of the cdsu
 * \return the initialized t_cdsu
 * \ingroup cdsu
**/
t_cdsu *cdsu_init(t_cdsu *to_init, size_t n) {
	void *buffer;

	if (!n || !to_init) {
		LOG(C_MINI__CDSU_LOG_EARG);
		return (NULL);
	}
	buffer = calloc(sizeof(int), n * 2);
	if (!buffer) {
		LOG(C_MINI__CDSU_LOG_EALLOC);
		return (NULL);
	}
	cdsu_init_from_arrays(to_init, n, buffer, buffer + n * sizeof(int));
	to_init->rp_single_alloc = true;
	LOG("New DSU (cdsu:%p, size:%zu)", to_init, n);
	return (to_init);
}
/**
 * \brief init by allocating a t_cdsu and using provided values
 * \param size size of the cdsu
 * \param parents the parents array
 * \param ranks the ranks array
 * \return the new t_cdsu
 * \ingroup cdsu
**/
t_cdsu *cdsu_new_from_array(size_t size, int *parents, int *ranks) {
	t_cdsu *ret;

	if (!size || !parents || !ranks) {
		LOG(C_MINI__CDSU_LOG_EARG);
		return (NULL);
	}
	ret = malloc(sizeof(t_cdsu));
	if (!ret) {
		LOG(C_MINI__CDSU_LOG_EALLOC);
		return (NULL);
	}
	ret->rank            = (uint *)ranks;
	ret->parent          = (uint *)parents;
	ret->size            = size;
	ret->rp_single_alloc = false;
	ret->components      = size;
	cdsu_clr(ret);
	LOG("New DSU (cdsu:%p, size:%zu)", ret, size);
	return (ret);
}
/**
 * \brief init t_cdsu using provided values (no allocs)
 * \param ret the t_cdsu to init
 * \param size size of the cdsu
 * \param parents the parents array
 * \param ranks the ranks array
 * \return the initialized t_cdsu
 * \ingroup cdsu
**/
t_cdsu *cdsu_init_from_arrays(t_cdsu *ret, size_t size, int *parents,
                              int *ranks) {
	if (!ret || !size || !parents || !ranks) {
		LOG(C_MINI__CDSU_LOG_EARG);
		return (NULL);
	}
	ret->rank            = (uint *)ranks;
	ret->parent          = (uint *)parents;
	ret->size            = size;
	ret->components      = size;
	ret->rp_single_alloc = false;
	cdsu_clr(ret);
	LOG("New DSU (cdsu:%p, size:%zu)", ret, size);
	return (ret);
}
/**
 * \brief grow the cdsu by N values
 *
 * (warning: will realocate. if you used local buffers durring construction
 * via from_arrays methods do not use).
 * \param cdsu the t_cdsu to grow
 * \param n the number of values to add
 * \return the grown t_cdsu
 * \ingroup cdsu
**/
t_cdsu *cdsu_grow(t_cdsu *cdsu, size_t n) {
	void  *replacement_buffer;
	void  *rank_offset;
	size_t i;

	if (!cdsu || !n) {
		LOG(C_MINI__CDSU_LOG_EARG);
		return (cdsu);
	}
	if (!cdsu->size || !cdsu->rank || !cdsu->parent) return (cdsu);
	replacement_buffer = malloc(sizeof(int) * (cdsu->size + n) * 2);
	if (!replacement_buffer) {
		LOG(C_MINI__CDSU_LOG_EALLOC);
		return (cdsu);
	}
	rank_offset = replacement_buffer + (cdsu->size + n) * sizeof(int);
	memcpy(replacement_buffer, cdsu->parent, cdsu->size * sizeof(int));
	memcpy(rank_offset, cdsu->rank, cdsu->size * sizeof(int));
	free(cdsu->parent);
	cdsu->parent = replacement_buffer;
	if (!cdsu->rp_single_alloc) free(cdsu->rank);
	cdsu->rank            = rank_offset;
	cdsu->rp_single_alloc = true;
	i                     = 0;
	while (i < n) {
		cdsu->parent[i + cdsu->size] = i + cdsu->size;
		cdsu->rank[i++ + cdsu->size] = 0;
	}
	LOG("Grow DSU (cdsu:%p, size:%zu -> %zu)", cdsu, cdsu->size,
	    cdsu->size + n);
	cdsu->size       += n;
	cdsu->components += n;
	return (cdsu);
}
