#include <stdlib.h>
#include <string.h>

#include "vtls.h"

/**
 * \brief remove the value associated to v_name
 * \param v_name name of the variable to search for
 * \return true if var was removed, false in any other case
**/
bool vtls_rm(const char *v_name) {
	t_vtls_array *vtls = pthread_getspecific(vtls_key);
	size_t        i    = 0;

	if (!vtls) {
		vtls = calloc(1, sizeof(*vtls));
		pthread_setspecific(vtls_key, vtls);
	}
	for (i = 0; i < vtls->len; i++)
		if (!strcmp(vtls->data[i].v_name, v_name)) break;
	if (strcmp(vtls->data[i].v_name, v_name)) return (false);
	memcpy(&vtls->data[i], &vtls->data[vtls->len - 1], sizeof(t_vtls));
	vtls->len--;
	return (true);
}
