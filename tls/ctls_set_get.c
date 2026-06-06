#include <pthread.h>
#include <stddef.h>
#include "ctls.h"

/**
 * \brief update value in a variable or create it with init_val
 * \param name the name of the variable to search for
 * \param init_val the value to set in type-safe wrapper
 * \return true if success, false otherwise
**/
bool ctls_set(const char *name, t_ctls_typewrap init_val) {
	t_ctlsv *ctls = pthread_getspecific(ctls_key);
	size_t   i;

	if (!ctls) {
		ctls = calloc(1, sizeof(*ctls));
		pthread_setspecific(ctls_key, ctls);
	}
	for (i = 0; i < ctls->len; i++) {
		if (!strcmp(name, ctls->v_name[i]))
			return (ctls->vals[i] = init_val, true);
	}
	if (i != TLS_CTLS_CAP) {
		ctls->v_name[ctls->len] = name;
		ctls->vals[ctls->len++] = init_val;
		return (true);
	}
	return (false);
}
