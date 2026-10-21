#ifndef C_MINI__VTLS
#define C_MINI__VTLS

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>

#include "common_base/log.h"

/**
 * \defgroup vtls Voided-type Thread Local Storage
**/

#ifdef C_MINI__VTLS_INTERNAL
#	ifndef C_MINI__VTLS_START_SIZE

#		ifdef C_MINI__TESTS
#			define C_MINI__VTLS_START_SIZE 4
#		else // || C_MINI__TESTS
#			define C_MINI__VTLS_START_SIZE 16
#		endif // !C_MINI__TESTS

#	endif // !C_MINI__VTLS_START_SIZE

/**
 * \typedef t_vtls
 * \brief the vtls key-value structure
 * \var v_name the name of the variable
 * \var var the value to associate the name with, (memory ptr)
 * \var size_e the size of the data to save
 * \ingroup vtls
**/
typedef struct s_vtls {
	const char *v_name;
	void       *var;
	size_t      size_e;
} t_vtls;

/**
 * \typedef t_vtls_array
 * \brief the vtls key-value holder structure
 * \var data the array of key-values
 * \var len the length of current key-value
 * \var cap maximum number of elements
 * \ingroup vtls
**/
typedef struct s_tlsv_array {
	t_vtls *data;
	size_t  len;
	size_t  cap;
} t_vtls_array;

/**
 * \var vtls_key tls key for the vtls system
 * \ingroup vtls
**/
extern pthread_key_t vtls_key;
#endif // C_MINI__VTLS_INTERNAL
// TODO: rename vtls -> vtls_key, vtls_array->vtls

bool vtls_rm(const char *v_name);

void  *vtls_get(const char *v_name);
// TODO: vtls_getout(const char *key, void *out);
// - memcpy to out if found using size of key'ed elem
size_t vtls_len(void);

bool vtls_has(const char *v_name);

bool vtls_set(const char *v_name, void *v_val, size_t v_size);

#endif // C_MINI__VTLS
