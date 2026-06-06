#include <stdlib.h>
#include <string.h>

#include "voided_thread_local_storage.h"

/**
 * \brief set a new value to existing variable, if not found create a new one
 * \param v_name the variable name
 * \param v_val the value of the variable
 * \param v_size the size of the value
 * \return true if value was added to the vtls, false in any other case
**/
bool vtls_set(const char *v_name, void *v_val, size_t v_size) {
	t_vtls_array *vtls = pthread_getspecific(vtls_key);
	size_t        i    = 0;

	if (!v_name) return (false);
	if (!vtls) {
		vtls = calloc(1, sizeof(*vtls));
		pthread_setspecific(vtls_key, vtls);
	}
	for (i = 0; i < vtls->len; i++) {
		if (vtls->data[i].size_e != v_size) continue;
		if (!strcmp(vtls->data[i].v_name, v_name))
			return (memcpy(&vtls->data[i].var, v_val, v_size), true);
	}
	if (vtls->len == vtls->cap) { // grow
		i = vtls->cap;
		if (!realloc(vtls->data, sizeof(t_vtls) * (!i ? 16 : i) * 2))
			return (false);
		vtls->cap = (!i ? 16 : i) * 2;
	}
	vtls->data[vtls->len++] =
	    (t_vtls){.var = v_val, .size_e = v_size, .v_name = v_name};
	return (true);
}
