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
	t_cdsu   *ret;
	uint16_t *buffer;

	if (!n) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return ((t_cdsu *)NULL);
	}
	// note: left alloc split for growth cappabilities.
	// otherwise growth function would need to also re-alloc the control struct
	ret = (t_cdsu *)malloc(sizeof(*ret));
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__CDSU_LOG_EALLOC)
		return ((t_cdsu *)NULL);
	}
	*ret   = (t_cdsu){0};
	buffer = malloc(sizeof(uint16_t) * n * 2);
	if (!buffer) {
		free(ret);
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__CDSU_LOG_EALLOC)
		return ((t_cdsu *)NULL);
	}
	cdsu_init_from_arrays(ret, n, buffer, (buffer + n));
	ret->rp_single_alloc = true;
	CM_LOG(C_MINI__LEVEL_INFO, "New DSU (cdsu:%p, size:%zu)", (void *)ret, n)
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
	uint16_t *buffer;

	if (!n || !to_init) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return ((t_cdsu *)NULL);
	}
	buffer = calloc(sizeof(uint16_t), n * 2);
	if (!buffer) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__CDSU_LOG_EALLOC)
		return ((t_cdsu *)NULL);
	}
	cdsu_init_from_arrays(to_init, n, buffer, buffer + n);
	to_init->rp_single_alloc = true;
	CM_LOG(C_MINI__LEVEL_INFO, "New DSU (cdsu:%p, size:%zu)", (void *)to_init,
	       n)
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
t_cdsu *cdsu_new_from_array(size_t size, uint16_t *parents, uint16_t *ranks) {
	t_cdsu *ret;

	if (!size || !parents || !ranks || ranks == parents) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return ((t_cdsu *)NULL);
	}
	ret = (t_cdsu *)malloc(sizeof(t_cdsu));
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__CDSU_LOG_EALLOC)
		return ((t_cdsu *)NULL);
	}
	ret->rank            = (uint16_t *)ranks;
	ret->parent          = (uint16_t *)parents;
	ret->size            = size;
	ret->components      = 1;
	ret->rp_single_alloc = false;
	cdsu_clr(ret);
	CM_LOG(C_MINI__LEVEL_INFO, "New DSU (cdsu:%p, size:%zu)", (void *)ret, size)
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
t_cdsu *cdsu_init_from_arrays(t_cdsu *ret, size_t size, uint16_t *parents,
                              uint16_t *ranks) {
	if (!ret || !size || !parents || !ranks || parents == ranks) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return ((t_cdsu *)NULL);
	}
	ret->rank            = (uint16_t *)ranks;
	ret->parent          = (uint16_t *)parents;
	ret->size            = size;
	ret->components      = 1;
	ret->rp_single_alloc = false;
	cdsu_clr(ret);
	CM_LOG(C_MINI__LEVEL_INFO, "New DSU (cdsu:%p, size:%zu)", (void *)ret, size)
	return (ret);
}
/**
 * \brief grow the cdsu by N values
 *
 * (warning: will realocate. if you used local buffers durring construction
 * via from_arrays methods do not use).
 * \param cdsu the t_cdsu to grow
 * \param n the number of values to add
 * \return true if the dsu has grown at least n spaces, false otherwise
 * \ingroup cdsu
**/
bool cdsu_grow(t_cdsu *cdsu, size_t n) {
	uint16_t *replacement_buffer;
	uint16_t *rank_offset;
	size_t    i;

	if (!cdsu || !cdsu->size || !cdsu->rank || !cdsu->parent) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CDSU_LOG_EARG)
		return (false);
	}
	if (!n) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CDSU_LOG_SUCCESS)
		return (true);
	}

	replacement_buffer = malloc(sizeof(uint16_t) * (cdsu->size + n) * 2);
	if (!replacement_buffer) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__CDSU_LOG_EALLOC)
		return (false);
	}
	rank_offset = replacement_buffer + (cdsu->size + n);
	memcpy(replacement_buffer, cdsu->parent, cdsu->size * sizeof(uint16_t));
	memcpy(rank_offset, cdsu->rank, cdsu->size * sizeof(uint16_t));
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
	CM_LOG(C_MINI__LEVEL_INFO, "Grow DSU (cdsu:%p, size:%zu -> %zu)",
	       (void *)cdsu, cdsu->size, cdsu->size + n)
	cdsu->size       += n;
	cdsu->components += n;
	return (true);
}
