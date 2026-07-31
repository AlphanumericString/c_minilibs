#define C_MINI__VTLS_INTERNAL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "voided_thread_local_storage.h"

/**
 * \brief set a new value to existing variable, if not found create a new one
 * \param v_name the variable name
 * \param v_val the value of the variable
 * \param v_size the size of the value
 * \return true if value was added to the vtls, false in any other case
 * \ingroup vtls
**/
bool vtls_set(const char *v_name, void *v_val, size_t v_size) {
	t_vtls_array *vtls = pthread_getspecific(vtls_key);
	size_t        i    = 0;
	t_vtls       *grow;

	if (!v_name) return (false);
	if (!vtls) {
		vtls = calloc(1, sizeof(*vtls));
		pthread_setspecific(vtls_key, vtls);
	}
	for (i = 0; i < vtls->len; i++) { // check already present=> update
		if (strcmp(vtls->data[i].v_name, v_name) != 0) continue;
		if (vtls->data[i].size_e != v_size) vtls->data[i].size_e = v_size;
		return (memcpy(&vtls->data[i].var, &v_val, sizeof(void *)), true);
	}
	if (vtls->len == vtls->cap) { // check space => grow
		i    = (!vtls->cap ? C_MINI__VTLS_START_SIZE : vtls->cap * 2);
		grow = realloc(vtls->data, sizeof(t_vtls) * i);
		if (!grow) return (false);
		vtls->data = grow;
		vtls->cap  = i;
	}
	vtls->data[vtls->len++] = (t_vtls){
	    .var    = v_val,
	    .size_e = v_size,
	    .v_name = v_name,
	};
	return (true);
}
