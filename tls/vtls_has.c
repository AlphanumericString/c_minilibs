#define C_MINI__VTLS_INTERNAL
#include <string.h>

#include "voided_thread_local_storage.h"

/**
 * \brief checks whether there is a variable of name v_name in memory
 * \param v_name the name of the variable to search for
 * \return true if found, false otherwise
 * \ingroup vtls
**/
bool vtls_has(const char *v_name) {
	const t_vtls *vtls = pthread_getspecific(vtls_key);

	if (!v_name) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_EARG);
		return (false);
	}
	if (!vtls) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_FAILLURE);
		return (false);
	}
	for (size_t i = 0; i < vtls->len; i++)
		if (!strcmp(vtls->data[i].v_name, v_name)) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_SUCCESS);
			return (true);
		}
	CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_FAILLURE);
	return (false);
}
