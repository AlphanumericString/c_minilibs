#define C_MINI__INTERNAL
#include <pthread.h>

#include "common_thread_local_storage.h"

pthread_key_t ctls_key;

static void __attribute__((destructor())) destroy_ctls(void) {
	t_ctlsv *ctls = pthread_getspecific(ctls_key);

	if (ctls) free(ctls);
	pthread_key_delete(ctls_key);
}

static void free_non_null(void *a) {
	if (a) free(a);
}

static void __attribute__((constructor())) init_ctls(void) {
	pthread_key_create(&ctls_key, free_non_null);
}

/**
 * \brief dumps the value contained in a type-safe wrapper
 * \param t the type to dump the value of
 * \param fd the filedescriptor where to output
 * \note this dumps the value directly to fd and is viewed as a debug tool
 * \ingroup ctls
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
