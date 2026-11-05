#include <stdint.h>
#include <string.h>
#define C_MINI__VTLS_INTERNAL
#include <pthread.h>
#include <stdlib.h>

#include "voided_thread_local_storage.h"

pthread_key_t vtls_key;

#define COV_IGNORE __attribute__((no_profile_instrument_function))
// note: coverage marks function as not running but mem-check tools clearly
// states that if absent memory leaks occur on multi-threaded examples.
// TLDR: ignore coverage report, bug on cov report toolchain

static void COV_IGNORE vtls_cleanup(void *a) {
	t_cm_vtls *vtls = a;

	if (vtls->data) free(vtls->data);
	free(vtls);
}
void COV_IGNORE __attribute__((destructor())) destroy_vtls_array(void) {
	t_cm_vtls *vtls = pthread_getspecific(vtls_key);

	if (vtls) vtls_cleanup(vtls);
	pthread_key_delete(vtls_key);
}
void __attribute__((constructor())) init_cm_vtls(void) {
	pthread_key_create(&vtls_key, vtls_cleanup);
}

/**
 * \brief dumps a specific vtls variable at a designated fd
 * \param v_name the name of the variable to print
 * \param fd the place where to print all vtls infos
 * \ingroup vtls
**/
bool vtls_dump(const char *v_name, int fd) {
	const t_cm_vtls *vtls = pthread_getspecific(vtls_key);

	if (!v_name || fd < 0) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_EARG);
		return (false);
	}
	if (!vtls) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_FAILLURE);
		return (false);
	}
	for (size_t i = 0; i < vtls->len; i++) {
		if (vtls->data[i].v_name && !strcmp(v_name, vtls->data[i].v_name)) {
			dprintf(fd, "%s:{ %16p %zu }\n", vtls->data[i].v_name,
			        vtls->data[i].var, vtls->data[i].size_e);
			CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_SUCCESS);
			return (true);
		}
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_FAILLURE);
	return (false);
}
/**
 * \brief dumps all vtls values at a designated fd
 * \param fd the place where to print all vtls infos
 * \ingroup vtls
**/
ssize_t vtls_dump_all(int fd) {
	const t_cm_vtls *vtls = pthread_getspecific(vtls_key);

	if (fd < 0) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_EARG);
		return (-1);
	}
	if (!vtls) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_FAILLURE);
		return (0);
	}
	for (size_t i = 0; i < vtls->len; i++) {
		dprintf(fd, "%s:{ %16p %zu }\n", vtls->data[i].v_name,
		        vtls->data[i].var, vtls->data[i].size_e);
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__VTLS_SUCCESS);
	return (vtls->len);
}
