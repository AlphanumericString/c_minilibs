#include <stdlib.h>
#include "voided_thread_local_storage.h"

pthread_key_t vtls_key;

void __attribute__((constructor())) init_vtls(void) {
	pthread_key_create(&vtls_key, free);
}
