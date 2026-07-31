#ifndef C_MINI__FTLS
#define C_MINI__FTLS

#include <pthread.h>

#ifdef C_MINI__TESTS
#	include <stdio.h>

#	ifndef LOG
#		define LOG(fmt, ...)                                                  \
			do {                                                               \
				printf("\e[33m[LOG]\e[0m: %s: " fmt "\n",                      \
				       __func__ __VA_OPT__(, ) __VA_ARGS__);                   \
				fflush(stdout);                                                \
			} while (0);
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...)                                                  \
			do {                                                               \
				printf("\e[36;1m[DBG]\e[0m: %s: " fmt "\n",                    \
				       __func__ __VA_OPT__(, ) __VA_ARGS__);                   \
				fflush(stdout);                                                \
			} while (0);
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS                                                        \
			do {                                                               \
				printf("\e[34;0m[PATH]\e[0m: %s:%d %s\n", __FILE__, __LINE__,  \
				       __FUNCTION__);                                          \
				fflush(stdout);                                                \
			} while (0);
#	endif // !DBG_POS

#else // || C_MINI__TESTS

#	ifndef LOG
#		define LOG(fmt, ...) (void)0;
#	endif // !LOG

#	ifndef DBG
#		define DBG(fmt, ...) (void)0;
#	endif // !DBG

#	ifndef DBG_POS
#		define DBG_POS (void)0;
#	endif // !DBG_POS

#endif // !C_MINI__TESTS
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

#if defined (C_MINI__FTLS_INTERNAL) || defined (C_MINI__FTLS_GET)
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

static void __attribute__((destructor())) ftls_dtor(void) {
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
		pthread_setspecific(ftls_key, ftls);
	}
	return (ftls);
}
#endif // !C_MINI__FTLS_GET

#endif // !C_MINI__FTLS
