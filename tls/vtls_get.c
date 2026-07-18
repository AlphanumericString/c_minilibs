#define C_MINI__INTERNAL
#include <string.h>

#include "voided_thread_local_storage.h"

/**
 * \brief return the value associated to v_name
 * \param v_name name of the variable to search for
 * \return the value associated to var
 * \ingroup vtls
**/
void *vtls_get(const char *v_name) {
	t_vtls_array *vtls = pthread_getspecific(vtls_key);
	size_t        i    = 0;

	if (!vtls || !v_name) return (NULL);
	for (i = 0; i < vtls->len; i++)
		if (!strcmp(vtls->data[i].v_name, v_name)) return (vtls->data[i].var);
	return (NULL);
}

/**
 * \brief return the number of vtls key stored
 * \return 0 if no vtls, otherwise the number of key stored inside
 * \ingroup vtls
**/
size_t vtls_len(void) {
	t_vtls_array *vtls = pthread_getspecific(vtls_key);

	if (!vtls) return (0);
	return (vtls->len);
}
