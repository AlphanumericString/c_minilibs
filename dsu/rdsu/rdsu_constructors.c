#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "rewindable_disjointed_set_union.h"

/**
 * \brief init by allocating both the return and the internals
 * \param n size of the rdsu
 * \return the new t_cm_rdsu
 * \ingroup rdsu
**/
t_cm_rdsu *rdsu_new(size_t n) {
	t_cm_rdsu *ret;
	uint16_t  *buffer;

	if (!n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return ((t_cm_rdsu *)NULL);
	}
	ret = (t_cm_rdsu *)malloc(sizeof(*ret));
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RDSU_LOG_EALLOC)
		return ((t_cm_rdsu *)NULL);
	}
	*ret   = (t_cm_rdsu){0};
	buffer = (uint16_t *)malloc(sizeof(uint16_t) * n * 2);
	if (!buffer) {
		free(ret);
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RDSU_LOG_EALLOC)
		return ((t_cm_rdsu *)NULL);
	}
	rdsu_init_from_arrays(ret, n, buffer, buffer + n);
	ret->rp_single_alloc = true;
	CM_LOG(C_MINI__LEVEL_INFO, "New DSU (rdsu:%p, size:%zu)", (void *)ret, n)
	return (ret);
}
/**
 * \brief init by allocating the internals
 * \param to_init the t_cm_rdsu to init
 * \param n size of the rdsu
 * \return the initialized t_cm_rdsu
 * \ingroup rdsu
**/
t_cm_rdsu *rdsu_init(t_cm_rdsu *to_init, size_t n) {
	uint16_t *buffer;

	if (!n || !to_init) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return ((t_cm_rdsu *)NULL);
	}
	buffer = (uint16_t *)calloc(sizeof(uint16_t), n * 2);
	if (!buffer) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RDSU_LOG_EALLOC)
		return ((t_cm_rdsu *)NULL);
	}
	rdsu_init_from_arrays(to_init, n, buffer, buffer + n);
	to_init->rp_single_alloc = true;
	CM_LOG(C_MINI__LEVEL_INFO, "New DSU (rdsu:%p, size:%zu)", (void *)to_init,
	       n)
	return (to_init);
}
/**
 * \brief init by allocating a t_cm_rdsu and using provided values
 * \param size size of the rdsu
 * \param parents the parents array
 * \param ranks the ranks array
 * \return the new t_cm_rdsu
 * \ingroup rdsu
**/
t_cm_rdsu *rdsu_new_from_arrays(size_t size, uint16_t *parents,
                                uint16_t *ranks) {
	t_cm_rdsu *ret;

	if (!size || !parents || !ranks) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return ((t_cm_rdsu *)NULL);
	}
	ret = (t_cm_rdsu *)malloc(sizeof(t_cm_rdsu));
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RDSU_LOG_EALLOC)
		return ((t_cm_rdsu *)NULL);
	}
	ret->rank            = (uint16_t *)ranks;
	ret->parent          = (uint16_t *)parents;
	ret->size            = size;
	ret->rp_single_alloc = false;
	ret->change_log      = NULL;
	ret->reserved_logs   = NULL;
	ret->components      = size;
	rdsu_clr(ret);
	CM_LOG(C_MINI__LEVEL_INFO, "New DSU (rdsu:%p, size:%zu)", (void *)ret, size)
	return (ret);
}
/**
 * \brief init t_cm_rdsu using provided values (no allocs)
 * \param ret the t_cm_rdsu to init
 * \param size size of the rdsu
 * \param parents the parents array
 * \param ranks the ranks array
 * \return the initialized t_cm_rdsu
 * \ingroup rdsu
**/
t_cm_rdsu *rdsu_init_from_arrays(t_cm_rdsu *ret, size_t size, uint16_t *parents,
                                 uint16_t *ranks) {
	if (!ret || !size || !parents || !ranks) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return ((t_cm_rdsu *)NULL);
	}
	ret->rank            = (uint16_t *)ranks;
	ret->parent          = (uint16_t *)parents;
	ret->size            = size;
	ret->components      = size;
	ret->rp_single_alloc = false;
	ret->change_log      = NULL;
	ret->reserved_logs   = NULL;
	rdsu_clr(ret);
	CM_LOG(C_MINI__LEVEL_INFO, "New DSU (rdsu:%p, size:%zu)", (void *)ret, size)
	return (ret);
}
/**
 * \brief grow the rdsu by N values
 *
 * (warning: will realocate. if you used local buffers durring construction
 * via from_arrays methods do not use).
 * \param rdsu the t_cm_rdsu to grow
 * \param n the number of values to add
 * \return the grown t_cm_rdsu
 * \ingroup rdsu
**/
bool rdsu_grow(t_cm_rdsu *rdsu, size_t n) {
	uint16_t *replacement_buffer;
	uint16_t *rank_offset;
	size_t    i;

	if (!rdsu || !n || !rdsu->size || !rdsu->rank || !rdsu->parent) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (false);
	}
	replacement_buffer = malloc(sizeof(uint16_t) * (rdsu->size + n) * 2);
	if (!replacement_buffer) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__RDSU_LOG_EALLOC)
		return (false);
	}
	rank_offset = replacement_buffer + (rdsu->size + n);
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
	CM_LOG(C_MINI__LEVEL_INFO, "Grow DSU (rdsu:%p, size:%zu -> %zu)",
	       (void *)rdsu, rdsu->size, rdsu->size + n)
	rdsu->size       += n;
	rdsu->components += n;
	return (true);
}
