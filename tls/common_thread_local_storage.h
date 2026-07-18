#ifndef C_MINI__CTLS
#define C_MINI__CTLS

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
 * \defgroup ctls C-types Thread Local Storage
**/

#ifndef C_MINI__CTLS_CAP
#	define C_MINI__CTLS_CAP 16
#endif

/**
 * 
 * \ingroup ctls
**/
extern pthread_key_t ctls_key;

/**
 * 
 * \ingroup ctls
**/
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

/**
 * \brief the array of C-type Thread Local Storage. Fast but limited to c-types.
 * \param v_name array of name for the different entries.
 * \param vals array of values for the different entries.
 * \param len lenght of the array curently being used.
 * \note if you want to increase the CTLS_CAP it's STRONGLY reccomended to not
 * go over 256
 * \ingroup ctls
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
void ctls_dump_typewrap(t_ctls_typewrap t, int fd);

#endif // !C_MINI__CTLS
