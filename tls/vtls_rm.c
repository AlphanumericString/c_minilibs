#define C_MINI__VTLS_INTERNAL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "voided_thread_local_storage.h"

/**
 * \brief remove the value associated to v_name
 * \param v_name name of the variable to search for
 * \return true if var was removed, false in any other case
 * \ingroup vtls
**/
bool vtls_rm(const char *v_name) {
	t_vtls *vtls = pthread_getspecific(vtls_key);
	size_t  i    = 0;

	if (!v_name) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_EARG);
		return (false);
	}
	if (!vtls || !vtls->len) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_FAILLURE);
		return (false);
	}
	for (i = 0; i < vtls->len; i++)
		if (!strcmp(vtls->data[i].v_name, v_name)) break;
	if (i == vtls->len) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_FAILLURE);
		return (false);
	}
	memcpy(&(vtls->data[i]), &(vtls->data[vtls->len - 1]), sizeof(t_vtls_key));
	vtls->len--;
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_SUCCESS);
	return (true);
}
