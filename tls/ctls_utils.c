#include "common_thread_local_storage.h"

pthread_key_t ctls_key;

void __attribute__((constructor())) init_ctls(void) {
	pthread_key_create(&ctls_key, free);
}

/**
 * \brief dumps the value contained in a type-safe wrapper
 * \param t the type to dump the value of
 * \note this dumps the value directly to console and is viewed as a debug tool
 **/
void ctls_dump_typewrap(t_ctls_typewrap t, int fd) {
	switch (t.type) {
	case CTLS_INT:
		dprintf(fd, "{ \"%s\", %d }", "int", t.value.i);
		break;
	case CTLS_CHAR:
		dprintf(fd, "{ \"%s\", %c (%d) }", "char", t.value.c, t.value.c);
		break;
	case CTLS_LONG:
		dprintf(fd, "{ \"%s\", %ld }", "long", t.value.l);
		break;
	case CTLS_PTR:
		dprintf(fd, "{ \"%s\", %p }", "ptr", t.value.p);
		break;
	case CTLS_NONE:
		dprintf(fd, "{ \"%s\", %p }", "none", t.value.p);
		break;
	default:
		dprintf(fd, "{ \"%s\" }", "unknown");
	}
}
