#define C_MINI__VTLS_INTERNAL
#include <string.h>

#include "voided_thread_local_storage.h"

/**
 * \brief return the value associated to v_name
 * \param v_name name of the variable to search for
 * \return the value associated to v_name
 * \ingroup vtls
**/
void *vtls_get(const char *v_name) {
	const t_vtls *vtls = pthread_getspecific(vtls_key);
	size_t        i    = 0;

	if (!vtls || !v_name) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_EARG);
		return (NULL);
	}
	for (i = 0; i < vtls->len; i++)
		if (!strcmp(vtls->data[i].v_name, v_name)) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_SUCCESS);
			return (vtls->data[i].var);
		}
	CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_FAILLURE);
	return (NULL);
}
/**
 * \brief return the number of vtls keys stored
 * \return 0 if no vtls, otherwise the number of keys stored inside
 * \ingroup vtls
**/
size_t vtls_len(void) {
	const t_vtls *vtls = pthread_getspecific(vtls_key);

	if (!vtls) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_FAILLURE);
		return (0);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_SUCCESS);
	return (vtls->len);
}
/**
 * \brief return the size of a stored value associated with a vtls key
 * \param v_name the name of the key to search
 * \return the size of the given key, or 0 if any error
 * \ingroup vtls
**/
size_t vtls_sizeof(const char *v_name) {
	const t_vtls *vtls = pthread_getspecific(vtls_key);
	size_t        i    = 0;

	if (!vtls || !v_name) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_EARG);
		return (0);
	}
	for (i = 0; i < vtls->len; i++)
		if (!strcmp(vtls->data[i].v_name, v_name)) {
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_SUCCESS);
			return (vtls->data[i].size_e);
		}
	CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_FAILLURE);
	return (0);
}
/**
 * \brief searches a key and if found loads memory to out buffer
 * \param v_name the name of the key to search
 * \param out the buffer to copy the value to
 * \return true if any match, false otherwise
 * \ingroup vtls
**/
bool vtls_getout(const char *v_name, void *out) {
	const t_vtls *vtls = pthread_getspecific(vtls_key);
	size_t        i    = 0;

	if (!vtls || !v_name) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_EARG);
		return (false);
	}
	for (i = 0; i < vtls->len; i++)
		if (!strcmp(vtls->data[i].v_name, v_name)) {
			memcpy(out, vtls->data[i].var, vtls->data[i].size_e);
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_SUCCESS);
			return (true);
		}
	CM_LOG(C_MINI__LEVEL_WARN, C_MINI__VTLS_FAILLURE);
	return (false);
}
