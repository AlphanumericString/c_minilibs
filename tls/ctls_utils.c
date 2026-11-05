#define C_MINI__CTLS_INTERNAL
#include <pthread.h>

#include "common_thread_local_storage.h"

pthread_key_t ctls_key;

#define COV_IGNORE __attribute__((no_profile_instrument_function))
// note: coverage marks function as not running but mem-check tools clearly
// states that if absent memory leaks occur on multi-threaded examples.
// TLDR: ignore coverage report, bug on cov report toolchain
static void COV_IGNORE __attribute__((destructor())) destroy_ctls(void) {
	t_cm_ctlsv *ctls = pthread_getspecific(ctls_key);

	if (ctls) free(ctls);
	pthread_key_delete(ctls_key);
}

static void __attribute__((constructor())) init_ctls(void) {
	pthread_key_create(&ctls_key, free);
}

/**
 * \brief dumps the value contained in a type-safe wrapper
 * \param t the type to dump the value of
 * \param fd the filedescriptor where to output
 * \note this dumps the value directly to fd and is viewed as a debug tool
 * \ingroup ctls
**/
bool ctls_dump_typewrap(t_cm_ctls_typewrap t, int fd) {
	if (fd < 0) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CTLS_EARG);
		return (false);
	}
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
	default:
		dprintf(fd, "{ \"%s\" }", "unknown");
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_SUCCESS);
	return (true);
}

/**
 * \brief dumps all values contained in a type-safe wrapper
 * \param fd the filedescriptor where to output
 * \note this dumps the values directly to fd
 * \ingroup ctls
**/
ssize_t ctls_dump_all(int fd) {
	const t_cm_ctlsv *ct;

	if (fd < 0) {
		CM_LOG(C_MINI__LEVEL_WARN, C_MINI__CTLS_EARG);
		return (-1);
	}
	ct = pthread_getspecific(ctls_key);
	if (!ct || !ct->len) {
		CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_SUCCESS);
		return (0);
	}
	for (size_t i = 0; i < ct->len; i++) {
		dprintf(fd, "%s:", ct->v_name[i]);
		ctls_dump_typewrap(ct->vals[i], fd);
		dprintf(fd, "\n");
	}
	CM_LOG(C_MINI__LEVEL_INFO, C_MINI__CTLS_SUCCESS);
	return (ct->len);
}
