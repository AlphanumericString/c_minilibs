#ifndef C_MINI__CTLS_H
#define C_MINI__CTLS_H

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define C_MINI__CTLS_CAP 16

static pthread_key_t tlsv_key;
static pthread_key_t ctls_key;

enum e_ctls_type {
	CTLS_NONE = 0,
	CTLS_CHAR,
	CTLS_INT,
	CTLS_LONG,
	CTLS_PTR,
};

/**
 * \brief type-sfafe abstraction of base c types.
 * \param type enum of c types.
 * \param value union of different possible interpretation of data.
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

/**
 * \brief the array of C-type Thread Local Storage. Fast but limited to c-types.
 * \param v_name array of name for the different entries
 * \param vals array of values for the different entries.
 * \param len lenght of the array curently being used.
 * \note if you want to increase the CTLS_CAP it's STRONGLY reccomended to not
 * go over 256
**/
typedef struct s_ctlsv {
	const char     *v_name[C_MINI__CTLS_CAP];
	t_ctls_typewrap vals[C_MINI__CTLS_CAP];
	size_t          len;
} t_ctlsv;

// remove
bool ctls_rm(const char *name);

// querry
bool ctls_has(const char *name);

// update / add
bool ctls_set(const char *name, t_ctls_typewrap init_val);

// get
t_ctls_typewrap *ctls_get(const char *name);
size_t           ctls_len(void);

// utils
void ctls_dump_typewrap(t_ctls_typewrap t);

#endif // CC_MINI__H
