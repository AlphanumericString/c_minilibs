#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "rewindable_disjointed_set_union.h"

/**
 * \brief init by allocating both the return and the internals
 * \param n size of the rdsu
 * \return the new t_rdsu
 * \ingroup rdsu
**/
t_rdsu *rdsu_new(size_t n) {
	t_rdsu *ret;
	void   *buffer;

	if (!n) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return (NULL);
	}
	ret = malloc(sizeof(*ret));
	if (!ret) {
		LOG(C_MINI__RDSU_LOG_EALLOC);
		return (NULL);
	}
	*ret   = (t_rdsu){0};
	buffer = malloc(sizeof(uint16_t) * n * 2);
	if (!buffer) {
		free(ret);
		LOG(C_MINI__RDSU_LOG_EALLOC);
		return (NULL);
	}
	rdsu_init_from_arrays(ret, n, buffer, buffer + n * sizeof(uint16_t));
	ret->rp_single_alloc = true;
	LOG("New DSU (rdsu:%p, size:%zu)", ret, n);
	return (ret);
}
/**
 * \brief init by allocating the internals
 * \param to_init the t_rdsu to init
 * \param n size of the rdsu
 * \return the initialized t_rdsu
 * \ingroup rdsu
**/
t_rdsu *rdsu_init(t_rdsu *to_init, size_t n) {
	void *buffer;

	if (!n || !to_init) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return (NULL);
	}
	buffer = calloc(sizeof(uint16_t), n * 2);
	if (!buffer) {
		LOG(C_MINI__RDSU_LOG_EALLOC);
		return (NULL);
	}
	rdsu_init_from_arrays(to_init, n, buffer, buffer + n * sizeof(uint16_t));
	to_init->rp_single_alloc = true;
	LOG("New DSU (rdsu:%p, size:%zu)", to_init, n);
	return (to_init);
}
/**
 * \brief init by allocating a t_rdsu and using provided values
 * \param size size of the rdsu
 * \param parents the parents array
 * \param ranks the ranks array
 * \return the new t_rdsu
 * \ingroup rdsu
**/
t_rdsu *rdsu_new_from_arrays(size_t size, uint16_t *parents, uint16_t *ranks) {
	t_rdsu *ret;

	if (!size || !parents || !ranks) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return (NULL);
	}
	ret = malloc(sizeof(t_rdsu));
	if (!ret) {
		LOG(C_MINI__RDSU_LOG_EALLOC);
		return (NULL);
	}
	ret->rank            = (uint16_t *)ranks;
	ret->parent          = (uint16_t *)parents;
	ret->size            = size;
	ret->rp_single_alloc = false;
	ret->change_log      = NULL;
	ret->components      = size;
	rdsu_clr(ret);
	LOG("New DSU (rdsu:%p, size:%zu)", ret, size);
	return (ret);
}
/**
 * \brief init t_rdsu using provided values (no allocs)
 * \param ret the t_rdsu to init
 * \param size size of the rdsu
 * \param parents the parents array
 * \param ranks the ranks array
 * \return the initialized t_rdsu
 * \ingroup rdsu
**/
t_rdsu *rdsu_init_from_arrays(t_rdsu *ret, size_t size, uint16_t *parents,
                              uint16_t *ranks) {
	if (!ret || !size || !parents || !ranks) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return (NULL);
	}
	ret->rank            = (uint16_t *)ranks;
	ret->parent          = (uint16_t *)parents;
	ret->size            = size;
	ret->components      = size;
	ret->rp_single_alloc = false;
	ret->change_log      = NULL;
	rdsu_clr(ret);
	LOG("New DSU (rdsu:%p, size:%zu)", ret, size);
	return (ret);
}
/**
 * \brief grow the rdsu by N values
 *
 * (warning: will realocate. if you used local buffers durring construction
 * via from_arrays methods do not use).
 * \param rdsu the t_rdsu to grow
 * \param n the number of values to add
 * \return the grown t_rdsu
 * \ingroup rdsu
**/
bool rdsu_grow(t_rdsu *rdsu, size_t n) {
	void  *replacement_buffer;
	void  *rank_offset;
	size_t i;

	if (!rdsu || !n || !rdsu->size || !rdsu->rank || !rdsu->parent) {
		LOG(C_MINI__RDSU_LOG_EARG);
		return (false);
	}
	replacement_buffer = malloc(sizeof(uint16_t) * (rdsu->size + n) * 2);
	if (!replacement_buffer) {
		LOG(C_MINI__RDSU_LOG_EALLOC);
		return (false);
	}
	rank_offset = replacement_buffer + (rdsu->size + n) * sizeof(uint16_t);
	memcpy(replacement_buffer, rdsu->parent, rdsu->size * sizeof(uint16_t));
	memcpy(rank_offset, rdsu->rank, rdsu->size * sizeof(uint16_t));
	free(rdsu->parent);
	rdsu->parent = replacement_buffer;
	if (!rdsu->rp_single_alloc) free(rdsu->rank);
	rdsu->rank            = rank_offset;
	rdsu->rp_single_alloc = true;
	i                     = 0;
	while (i < n) {
		rdsu->parent[i + rdsu->size] = i + rdsu->size;
		rdsu->rank[i++ + rdsu->size] = 0;
	}
	LOG("Grow DSU (rdsu:%p, size:%zu -> %zu)", rdsu, rdsu->size,
	    rdsu->size + n);
	rdsu->size       += n;
	rdsu->components += n;
	return (true);
}
