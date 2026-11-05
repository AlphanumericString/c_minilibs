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
	t_cm_vtls     *vtls = pthread_getspecific(vtls_key);
	size_t         i    = 0;
	t_cm_vtls_key *growth;

	if (!v_name) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_EARG);
		return (false);
	}
	if (!vtls) {
		vtls = calloc(1, sizeof(*vtls));
		if (!vtls) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__VTLS_EALLOC);
			return (false);
		}
		pthread_setspecific(vtls_key, vtls);
	}
	for (i = 0; i < vtls->len; i++) { // check already present=> update
		if (strcmp(vtls->data[i].v_name, v_name) != 0) continue;
		vtls->data[i].size_e = v_size;
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_SUCCESS);
		return (memcpy(&vtls->data[i].var, &v_val, sizeof(void *)), true);
	}
	if (vtls->len == vtls->cap) { // check space => grow
		i      = (!vtls->cap ? C_MINI__VTLS_START_SIZE : vtls->cap * 2);
		growth = realloc(vtls->data, sizeof(t_cm_vtls_key) * i);
		if (!growth) {
			CM_LOG(C_MINI__LEVEL_ERR, C_MINI__VTLS_EALLOC);
			return (false);
		}
		vtls->data = growth;
		vtls->cap  = i;
	}
	vtls->data[vtls->len++] = (t_cm_vtls_key){
	    .var    = v_val,
	    .size_e = v_size,
	    .v_name = v_name,
	};
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_SUCCESS);
	return (true);
}
