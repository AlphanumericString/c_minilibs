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
#	define C_MINI__FTLS_STRUCT struct s_tls_ftls_node
#endif

/**
 * \brief the ftls is the fastest of the tls and provide typesafety but can ONLY
 * be set at compile time..
 * \note please override with your own struct (or alternatively, name your own
 * struct s_tls_ftls_node
**/
typedef C_MINI__FTLS_STRUCT t_ftls;

#if defined(C_MINI__FTLS_INTERNAL) || defined(C_MINI__FTLS_GET)
/**
 * \brief key used for the f tls system. mandatory for thread management.
 * \ingroup ftls
**/
extern pthread_key_t ftls_key;
#endif // C_MINI__FTLS_INTERNAL

t_ftls *ftls_get(void);

#ifdef C_MINI__FTLS_GET
#	include <stdlib.h>

pthread_key_t ftls_key;

#	define COV_IGNORE __attribute__((no_profile_instrument_function))
// note: coverage marks function as not running but mem-check tools clearly
// states that if absent memory leaks occur on multi-threaded examples.
// TLDR: ignore coverage repport, bug on cov repport toolchain
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
void __attribute__((constructor())) init_ftls(void) {
	pthread_key_create(&ftls_key, free);
}

/**
 * \brief return the Thread Local Storage structure we attached.
 * \return the struct attached to the tls, null is alloc fails.
 * \note check the tests for an example.
 * \ingroup ftls
**/
t_ftls *ftls_get(void) {
	t_ftls *ftls = pthread_getspecific(ftls_key);

	if (!ftls) {
		ftls = calloc(1, sizeof(*ftls));
		if (!ftls) return (NULL);
		pthread_setspecific(ftls_key, ftls);
	}
	return (ftls);
}
#endif // !C_MINI__FTLS_GET

#endif // !C_MINI__FTLS
