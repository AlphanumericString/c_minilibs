#include "common_thread_local_storage.h"

/**
 * \brief check whether the local thread variable has s.
 * \param s the variable to check
 * \return true if exist no otherwise
 * \ingroup ctls
**/
bool ctls_has(const char *s) {
	t_ctlsv *ctls = pthread_getspecific(ctls_key);

	if (!ctls) return (false);
	for (size_t i = 0; i < ctls->len; i++) {
		if (!strcmp(s, ctls->v_name[i])) return (true);
	}
	return (false);
}
