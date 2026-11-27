#include <stdlib.h>
#include <strings.h>

#include "list_handler.h"

/**
 * \brief Initializes a list handler in place.
 * \param lh The handler to initialize.
 * \param type The underlying list type.
 * \param offset Offset of the intrusive node inside the user payload.
 * \return The initialized handler, or NULL on invalid arguments.
 * \ingroup lh
**/
t_cm_lh *lh_init(t_cm_lh *lh, t_cm_lh_type type, size_t offset) {
	if (!lh) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
	if (type != CM_LH_SINGLY && type != CM_LH_DOUBLY &&
	    type != CM_LH_CIRC_SINGLY && type != CM_LH_CIRC_DOUBLY) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return (NULL);
	}
	bzero(lh, sizeof(*lh));
	lh->offset = offset;
	lh->type   = type;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LH_LOG_SUCCESS)
	return (lh);
}

/**
 * \brief Creates a list handler on the heap.
 * \param type The underlying list type.
 * \param offset Offset of the intrusive node inside the user payload.
 * \return The new handler, or NULL on allocation failure.
 * \ingroup lh
**/
t_cm_lh *lh_create(t_cm_lh_type type, size_t offset) {
	t_cm_lh *ret;

	ret = malloc(sizeof(*ret));
	if (!ret) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LH_LOG_EALLOC)
		return (NULL);
	}
	if (!lh_init(ret, type, offset)) {
		CM_LOG(C_MINI__LEVEL_ERR, C_MINI__LH_LOG_EALLOC)
		free(ret);
		return (NULL);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LH_LOG_SUCCESS)
	return (ret);
}

/**
 * \brief Resets the handler metadata without freeing the handler or its
 * elements.
 * \param lh The handler to reset.
 * \ingroup lh
**/
void lh_destroy(t_cm_lh *lh) {
	if (!lh) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return;
	}
	lh->head = NULL;
	lh->tail = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LH_LOG_SUCCESS)
}

/**
 * \brief Destroys and frees a heap allocated handler.
 * \param lh Pointer to the handler to free.
 * \ingroup lh
**/
void lh_free(t_cm_lh **lh) {
	if (!lh || !*lh) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__LH_LOG_EARG)
		return;
	}
	lh_destroy(*lh);
	free(*lh);
	*lh = NULL;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__LH_LOG_SUCCESS)
}
