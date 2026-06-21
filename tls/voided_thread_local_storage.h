#ifndef C_MINI__VTLS_H
#define C_MINI__VTLS_H

#include <pthread.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct s_tlsv {
	const char *v_name;
	void       *var;    // mem ptr to var
	size_t      size_e; // len of memory of var
} t_vtls;

typedef struct s_tlsv_array {
	t_vtls *data;
	size_t  len;
	size_t  cap;
} t_vtls_array;

extern pthread_key_t vtls_key;

// remove
bool vtls_rm(const char *v_name);

// get
void *vtls_get(const char *v_name);
size_t vtls_len(void);

// querry
bool vtls_has(const char *v_name);

// update / add
bool vtls_set(const char *v_name, void *v_val, size_t v_size);

#endif // C_MINI__VTLS_H
