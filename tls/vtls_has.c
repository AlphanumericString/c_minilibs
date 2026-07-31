#define C_MINI__VTLS_INTERNAL
#include <string.h>

#include "voided_thread_local_storage.h"

/**
 * \brief checks whether there is a variable of name v_name in memory
 * \param v_name the name of the variable to search for
 * \return true if found, no otherwise
 * \ingroup vtls
**/
bool vtls_has(const char *v_name) {
	t_vtls_array *vtls = pthread_getspecific(vtls_key);

	if (!vtls || !v_name) return (false);
	for (size_t i = 0; i < vtls->len; i++)
		if (!strcmp(vtls->data[i].v_name, v_name)) return (true);
	return (false);
}
