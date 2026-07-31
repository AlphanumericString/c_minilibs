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
	t_vtls_array *vtls = pthread_getspecific(vtls_key);
	size_t        i    = 0;

	if (!vtls || !v_name) {
		return (false);
	}
	if (!vtls->len) return (false);
	for (i = 0; i < vtls->len; i++)
		if (!strcmp(vtls->data[i].v_name, v_name)) break;
	if (i == vtls->len) return (false);
	memcpy(&(vtls->data[i]), &(vtls->data[vtls->len - 1]), sizeof(t_vtls));
	vtls->len--;
	return (true);
}
