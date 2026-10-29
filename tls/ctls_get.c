#define C_MINI__CTLS_INTERNAL
#include <pthread.h>

#include "common_thread_local_storage.h"

/**
 * \brief try to get the local thread variable s.
 * \param s the name of the variable to get
 * \return the address of the variable in a type-safe wrapper, or NULL if not
 * found
 * \ingroup ctls
**/
t_ctls_typewrap *ctls_get(const char *s) {
	t_ctlsv *ctls = pthread_getspecific(ctls_key);

	if (!ctls) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CTLS_EARG);
		return (NULL);
	}
	for (size_t i = 0; i < ctls->len; i++) {
		if (!strcmp(s, ctls->v_name[i])) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_SUCCESS);
			return (&ctls->vals[i]);
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_FAILLURE);
	return (NULL);
}

/**
 * \brief length of the current ctls array
 * \return the length of the occupied ctls array
 * \ingroup ctls
**/
size_t ctls_len(void) {
	t_ctlsv *ctls = pthread_getspecific(ctls_key);

	if (!ctls) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_FAILLURE);
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_SUCCESS);
	return (ctls->len);
}
