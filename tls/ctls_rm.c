#define C_MINI__INTERNAL
#include <string.h>

#include "common_thread_local_storage.h"

/**
 * \brief try to remove name from the variables. (works by swap).
 * \param name name of the variable to rm
 * \return true if succeed, false in any ohter case.
 * \ingroup ctls
**/
bool ctls_rm(const char *name) {
	t_ctlsv *ctls = pthread_getspecific(ctls_key);

	if (!ctls) return (false);
	for (size_t i = 0; i < ctls->len; i++) {
		if (!strcmp(name, ctls->v_name[i])) {
			memcpy(&ctls->v_name[i], &ctls->v_name[ctls->len - 1],
			       sizeof(char *));
			memcpy(&ctls->vals[i], &ctls->vals[ctls->len - 1],
			       sizeof(t_ctls_typewrap));
			ctls->len--;
			return (true);
		}
	}
	return (false);
}
