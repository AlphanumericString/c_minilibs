#define C_MINI__CTLS_INTERNAL
#include <pthread.h>
#include <stddef.h>
#include <string.h>

#include "common_thread_local_storage.h"

/**
 * \brief update value in a variable or create it with init_val
 * \param name the name of the variable to search for
 * \param init_val the value to set in type-safe wrapper
 * \return true on success, false otherwise
 * \ingroup ctls
**/
bool ctls_set(const char *name, t_ctls_typewrap init_val) {
	t_ctlsv *ctls = pthread_getspecific(ctls_key);
	size_t   i;

	if (!ctls) {
		ctls = calloc(1, sizeof(*ctls));
		if (!ctls) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__CTLS_EALLOC)
			return (false);
		}
		pthread_setspecific(ctls_key, ctls);
	}
	for (i = 0; i < ctls->len; i++) {
		if (!strcmp(name, ctls->v_name[i])) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_SUCCESS)
			return (ctls->vals[i] = init_val, true);
		}
	}
	if (i != C_MINI__CTLS_CAP) {
		ctls->v_name[ctls->len] = name;
		ctls->vals[ctls->len]   = init_val;
		ctls->len              += 1;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_SUCCESS)
		return (true);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_FAILLURE)
	return (false);
}
