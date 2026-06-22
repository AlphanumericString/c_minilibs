#include <assert.h>
#include <tls/voided_thread_local_storage.h>
#include "tls_tests.h"

// update / add
void test_vtls_set(void){
	assert(vtls_set("a", (void *)42, sizeof(int)));
	assert(vtls_get("a") == (void *)42);
	assert(vtls_set("b", (void *)562, sizeof(int)));
	assert(vtls_get("b") == (void *)562);
	assert(vtls_get("a") == (void *)42);
	vtls_rm("a");
	vtls_rm("b");
	return ;
}
