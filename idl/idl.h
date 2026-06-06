#ifndef PRIV_IDL
#define PRIV_IDL

#include <stddef.h>

typedef struct s_idl {
	struct s_idl *prev;
	struct s_idl *nxt;
} t_idl;

// push/pop ?npush?
// to/from array
// peek / find / ?at?
// apply / apply with / apply it
// ?swap? / ?rm?
// clear?

#endif
