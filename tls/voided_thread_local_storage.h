#ifndef C_MINI__VTLS
#define C_MINI__VTLS

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>

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
 * \defgroup vtls Voided-type Thread Local Storage
**/

/**
 * \brief the vtls key-value structure
 * \param v_name the name of the variable
 * \param var the value to associate the name with
 * \param size_e the size of the data to save
 * \ingroup vtls
**/
typedef struct s_vtls {
	const char *v_name;
	void       *var;    // mem ptr to var
	size_t      size_e; // len of memory of var
} t_vtls;

/**
 * \brief the vtls key-value structure
 * \param data the array of key-values
 * \param len the lenght of current key-value
 * \param cap maximum number of elements
 * \ingroup vtls
**/
typedef struct s_tlsv_array {
	t_vtls *data;
	size_t  len;
	size_t  cap;
} t_vtls_array;

extern pthread_key_t vtls_key;

// remove
bool vtls_rm(const char *v_name);

// get
void  *vtls_get(const char *v_name);
size_t vtls_len(void);

// querry
bool vtls_has(const char *v_name);

// update / add
bool vtls_set(const char *v_name, void *v_val, size_t v_size);

#endif // C_MINI__VTLS
