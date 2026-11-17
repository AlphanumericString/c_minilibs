#include "rewindable_disjointed_set_union.h"

/**
 * \brief returns the max depth of the trees (the longest chain of parents from
 * any element up to its root)
 * \param rdsu the rdsu to query
 * \return the max depth, -1 on bad arguments
 * \note does not compress the rdsu so it does not invalidate the rewind logs
 * \ingroup rdsu
**/
int rdsu_max_depth(const t_cm_rdsu *rdsu) {
	unsigned int max = 0;

	if (!rdsu) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (-1);
	}
	for (size_t i = 0; i < rdsu->size; i++) {
		unsigned int depth = 0;

		for (size_t x = i; rdsu->parent[x] != x; x = rdsu->parent[x])
			depth++;
		if (depth > max) max = depth;
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (max);
}
/**
 * \brief returns the number of elems in the tree
 * \param rdsu the rdsu to query
 * \return the number of elems, 0 on bad arguments
 * \ingroup rdsu
**/
size_t rdsu_size(const t_cm_rdsu *rdsu) {
	if (!rdsu) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (rdsu->size);
}
/**
 * \brief returns the number of log entries reserved for future linkings
 * \param rdsu the rdsu to query
 * \return the number of reserved logs, 0 on bad arguments
 * \ingroup rdsu
**/
size_t rdsu_reserved_logs_nb(const t_cm_rdsu *rdsu) {
	if (!rdsu) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (rdsu->reserved_logs_nb);
}
/**
 * \brief return the number of distinct separate sets in the rdsu
 * \param rdsu the rdsu to query
 * \return the number of components, 0 on bad arguments
 * \ingroup rdsu
**/
size_t rdsu_nb_components(const t_cm_rdsu *rdsu) {
	if (!rdsu) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__RDSU_LOG_EARG)
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__RDSU_LOG_SUCCESS)
	return (rdsu->components);
}
