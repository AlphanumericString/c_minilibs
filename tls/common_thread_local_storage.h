#ifndef C_MINI__CTLS
#define C_MINI__CTLS

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common_base/log.h"

#ifdef C_MINI__TESTS

#	define C_MINI__CTLS_EARG     C_MINI__LOG_EARG
#	define C_MINI__CTLS_EALLOC   C_MINI__LOG_EALLOC
#	define C_MINI__CTLS_SUCCESS  C_MINI__LOG_SUCCESS
#	define C_MINI__CTLS_FAILLURE C_MINI__LOG_FAIL

#endif

/**
 * \defgroup ctls C-types Thread Local Storage
**/

#ifndef C_MINI__CTLS_CAP
#	ifdef C_MINI__TESTS
#		define C_MINI__CTLS_CAP 16
#	else
#		define C_MINI__CTLS_CAP 256
#	endif // !C_MINI__TESTS
#endif     // !C_MINI__CTLS_CAP

#ifdef C_MINI__CTLS_INTERNAL
/**
 * \var ctls_key tls key used by the ctls system
 * \ingroup ctls
**/
extern pthread_key_t ctls_key;
#endif // C_MINI__CTLS_INTERNAL

/**
 * \brief enum to typedef the ctls encapsulated types
 * \param CTLS_CHAR char representation value
 * \param CTLS_INT int representation value
 * \param CTLS_LONG long representation value
 * \param CTLS_PTR pointer representation value
 * \ingroup ctls
**/
enum e_ctls_type {
	CTLS_CHAR = 0,
	CTLS_INT,
	CTLS_LONG,
	CTLS_PTR,
};

/**
 * \brief type-safe abstraction of base c types for ctls system.
 * \param type enum of c types.
 * \param value union of different possible interpretation of data.
 * \ingroup ctls
**/
typedef struct {
	enum e_ctls_type type;
	union {
		char  c;
		int   i;
		long  l;
		void *p;
	} value;
} t_ctls_typewrap;

#ifdef C_MINI__CTLS_INTERNAL
/**
 * \brief the array of Thread Local Storage for C-type. Fast but limited to
 * c-types.
 * \param v_name array of names for the different entries.
 * \param vals array of values for the different entries.
 * \param len length of the array currently being used.
 * \note if you want to increase the CTLS_CAP it's STRONGLY recommended to not
 * go over 256
 * \ingroup ctls
**/
typedef struct s_ctlsv {
	const char     *v_name[C_MINI__CTLS_CAP];
	t_ctls_typewrap vals[C_MINI__CTLS_CAP];
	size_t          len;
} t_ctlsv;

#endif // C_MINI__CTLS_INTERNAL

bool             ctls_rm(const char *name);
bool             ctls_has(const char *name);
bool             ctls_set(const char *name, t_ctls_typewrap init_val);
t_ctls_typewrap *ctls_get(const char *name);
size_t           ctls_len(void);
bool             ctls_dump_typewrap(t_ctls_typewrap t, int fd);
ssize_t          ctls_dump_all(int fd);

#endif // !C_MINI__CTLS
