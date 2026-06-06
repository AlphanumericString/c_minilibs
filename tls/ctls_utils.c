#include "common_thread_local_storage.h"

void __attribute__((constructor())) init_ctls(void) {
	pthread_key_create(&ctls_key, free);
}

void __attribute__((destructor())) delete_ctls(void) {
	t_ctlsv *ctls = pthread_getspecific(ctls_key);

	if (ctls) free(ctls);
}

/**
 * \brief dumps the value contained in a type-safe wrapper
 * \param t the type to dump the value of
 * \note this dumps the value directly to console and is viewed as a debug tool
**/
void ctls_dump_typewrap(t_ctls_typewrap t) {
	switch (t.type) {
	case CTLS_INT:
		printf("{ \"%s\", %d }", "int", t.value.i);
		break;
	case CTLS_CHAR:
		printf("{ \"%s\", %c (%d) }", "cahr", t.value.c, t.value.c);
		break;
	case CTLS_LONG:
		printf("{ \"%s\", %ld }", "long", t.value.l);
		break;
	case CTLS_PTR:
		printf("{ \"%s\", %p }", "ptr", t.value.p);
		break;
	case CTLS_NONE:
		printf("{ \"%s\", %p }", "none", t.value.p);
		break;
	default:
		printf("{ \"%s\" }", "unknown");
	}
}
