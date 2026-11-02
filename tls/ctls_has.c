#define C_MINI__CTLS_INTERNAL
#include "common_thread_local_storage.h"

/**
 * \brief check whether the local thread variable contains s.
 * \param s the name of the variable to check
 * \return true if it exists, false otherwise
 * \ingroup ctls
**/
bool ctls_has(const char *s) {
	const t_ctlsv *ctls = pthread_getspecific(ctls_key);

	if (!ctls) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_FAILLURE);
		return (false);
	}
	for (size_t i = 0; i < ctls->len; i++) {
		if (!strcmp(s, ctls->v_name[i])) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_SUCCESS);
			return (true);
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_FAILLURE);
	return (false);
}
