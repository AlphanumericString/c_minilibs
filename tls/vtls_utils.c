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
	t_vtls_array *vtls = a;

	if (vtls->data) free(vtls->data);
	free(vtls);
}
void COV_IGNORE __attribute__((destructor())) destroy_vtls_array(void) {
	t_vtls_array *vtls = pthread_getspecific(vtls_key);

	if (vtls) vtls_cleanup(vtls);
	pthread_key_delete(vtls_key);
}

void __attribute__((constructor())) init_vtls(void) {
	pthread_key_create(&vtls_key, vtls_cleanup);
}
