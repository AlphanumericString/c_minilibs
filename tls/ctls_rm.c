#define C_MINI__CTLS_INTERNAL
#include <string.h>

#include "common_thread_local_storage.h"

/**
 * \brief try to remove name from the variables. (works by swap).
 * \param name name of the variable to remove
 * \return true on success, false otherwise.
 * \ingroup ctls
**/
bool ctls_rm(const char *name) {
	t_ctlsv *ctls = pthread_getspecific(ctls_key);

	if (!ctls) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CTLS_FAILLURE);
		return (false);
	}
	for (size_t i = 0; i < ctls->len; i++) {
		if (!strcmp(name, ctls->v_name[i])) {
			memcpy(&ctls->v_name[i], &ctls->v_name[ctls->len - 1],
			       sizeof(char *));
			memcpy(&ctls->vals[i], &ctls->vals[ctls->len - 1],
			       sizeof(t_ctls_typewrap));
			ctls->len--;
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_SUCCESS);
			return (true);
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_SUCCESS);
	return (false);
}
