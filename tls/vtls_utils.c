#define C_MINI__VTLS_INTERNAL
#include <pthread.h>
#include <stdlib.h>

#include "voided_thread_local_storage.h"

pthread_key_t vtls_key;

static void vtls_cleanup(void *a) {
	t_vtls_array *vtls = a;

	if (vtls->data) free(vtls->data);
	free(vtls);
}
void __attribute__((destructor())) destroy_vtls_array(void) {
	t_vtls_array *vtls = pthread_getspecific(vtls_key);

	if (vtls) vtls_cleanup(vtls);
	pthread_key_delete(vtls_key);
}

void __attribute__((constructor())) init_vtls(void) {
	pthread_key_create(&vtls_key, vtls_cleanup);
}
