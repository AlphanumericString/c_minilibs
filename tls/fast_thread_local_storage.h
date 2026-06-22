#ifndef C_MINI__FTLS_H
#	define C_MINI__FTLS_H

#include <pthread.h>

// +---------------------------------------------------+
// | OVERRIDE THIS TO SET YOUR OWN STRUCT NAME INSTEAD |
// +---------------------------------------------------+
#	ifndef C_MINI__FTLS_STRUCT
#		define C_MINI__FTLS_STRUCT struct s_tls_ftls_node
#	endif

/**
 * \brief the ftls is the fastest of the tls and provide typesafety but can ONLY
 * be set at compile time..
 * \note please override with your own struct (or alternatively, name your own
 * struct s_tls_ftls_node
 **/
typedef C_MINI__FTLS_STRUCT t_ftls;

/**
 * \brief key used for the f tls system. mandatory for thread management.
 **/
extern pthread_key_t ftls_key;

t_ftls *ftls_get(void);

#	ifdef C_MINI__FTLS_GET
#include <stdlib.h>

pthread_key_t ftls_key;

static void __attribute__((destructor())) ftls_dtor(void)
{
	void *p = pthread_getspecific(ftls_key);

	if (!p) return;
	free(p);
	pthread_key_delete(ftls_key);
}

static void ftls_cleanup(void *p)
{
	if (!p) return;
    free(p);
}
void __attribute__((constructor())) init_ftls(void) {
	pthread_key_create(&ftls_key, ftls_cleanup);
}

/**
 * \brief return the Thread Local Storage structure we attached.
 * \return the struct attached to the tls, null is alloc fails.
 * \note check the tests for an example.
 **/
t_ftls *ftls_get(void) {
	t_ftls *ftls = pthread_getspecific(ftls_key);

	if (!ftls) {
		ftls = calloc(1, sizeof(*ftls));
		pthread_setspecific(ftls_key, ftls);
	}
	return (ftls);
}
#	endif // C_MINI__FTLS_GET

#endif // FC_MINI__H
