#ifndef C_MINI__FTLS
#define C_MINI__FTLS

#include <pthread.h>

#include "common_base/log.h"

/**
 * \defgroup ftls Fast Thread Local Storage
**/

// +---------------------------------------------------+
// | OVERRIDE THIS TO SET YOUR OWN STRUCT NAME INSTEAD |
// +---------------------------------------------------+
#ifndef C_MINI__FTLS_STRUCT
#	define C_MINI__FTLS_STRUCT struct s_cm_tls_ftls_node
#endif

/**
 * \brief ftls is the fastest of the tls variants and provides type safety but
 * can ONLY be set at compile time.
 * \note please override with your own struct (or alternatively, name your own
 * struct s_cm_tls_ftls_node).
 * \ingroup ftls
**/
typedef C_MINI__FTLS_STRUCT t_cm_ftls;

#if defined(C_MINI__FTLS_INTERNAL) || defined(C_MINI__FTLS_GET)
/**
 * \var ftls_key tls key used for the ftls system.
 * \ingroup ftls
**/
extern pthread_key_t ftls_key;
#endif // C_MINI__FTLS_INTERNAL

t_cm_ftls *ftls_get(void);

#ifdef C_MINI__FTLS_GET
#	include <stdlib.h>

pthread_key_t ftls_key;

#	define COV_IGNORE __attribute__((no_profile_instrument_function))
// note: coverage marks function as not running but mem-check tools clearly
// states that if absent memory leaks occur on multi-threaded examples.
// TLDR: ignore coverage report, bug on cov report toolchain
static void COV_IGNORE __attribute__((destructor())) ftls_dtor(void) {
	void *p = pthread_getspecific(ftls_key);

	if (!p) return;
	free(p);
	pthread_key_delete(ftls_key);
}

/**
 * \brief init the Thread Local Storage structure.
 * \note check the tests for an example.
 * \ingroup ftls
**/
void __attribute__((constructor())) init_cm_ftls(void) {
	pthread_key_create(&ftls_key, free);
}

/**
 * \brief return the Thread Local Storage structure we attached.
 * \return the struct attached to the tls, NULL if allocation fails.
 * \note check the tests for an example.
 * \ingroup ftls
**/
t_cm_ftls *ftls_get(void) {
	t_cm_ftls *ftls = pthread_getspecific(ftls_key);

	if (!ftls) {
		ftls = calloc(1, sizeof(*ftls));
		if (!ftls) return (NULL);
		pthread_setspecific(ftls_key, ftls);
	}
	return (ftls);
}
#endif // !C_MINI__FTLS_GET

#endif // !C_MINI__FTLS
