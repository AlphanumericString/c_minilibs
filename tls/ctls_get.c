#define C_MINI__CTLS_INTERNAL
#include <pthread.h>

#include "common_thread_local_storage.h"

/**
 * \brief try to get the local thread variable s.
 * \param s the variable to check
 * \return the adress of the variable in a typesafe wrapper. if !s return NULL
 * \ingroup ctls
**/
t_ctls_typewrap *ctls_get(const char *s) {
	t_ctlsv *ctls = pthread_getspecific(ctls_key);

	if (!ctls) {
		return (NULL);
	}
	for (size_t i = 0; i < ctls->len; i++) {
		if (!strcmp(s, ctls->v_name[i])) {
			return (&ctls->vals[i]);
		}
	}
	return (NULL);
}

/**
 * \brief length of the current ctls array
 * \return the length of the occupied ctls array
 * \ingroup ctls
**/
size_t ctls_len(void) {
	t_ctlsv *ctls = pthread_getspecific(ctls_key);

	if (!ctls) return (0);
	return (ctls->len);
}
