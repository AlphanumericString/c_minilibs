#include <assert.h>
#include <unistd.h>

#include "tls_tests.h"

NO_COV void test_ctls_dump_typewrap(void) {
	const t_ctls_typewrap vars[] = {
	    (t_ctls_typewrap){.type = CTLS_INT,  .value.i = -1              },
	    (t_ctls_typewrap){.type = CTLS_CHAR, .value.c = 'd'             },
	    (t_ctls_typewrap){.type = 99,        .value.p = (void *)0xDEAD  },
	    (t_ctls_typewrap){.type = CTLS_PTR,  .value.l = (long)0xDEADBEEF},
	    (t_ctls_typewrap){.type = CTLS_PTR,  .value.i = (int)0x42       },
	    (t_ctls_typewrap){.type = CTLS_LONG, .value.l = (long)999       },
	};
	const char *expected = "{ \"int\", -1 }"
	                       "{ \"char\", d (100) }"
	                       "{ \"unknown\" }"
	                       "{ \"ptr\", 0xdeadbeef }"
	                       "{ \"ptr\", 0x42 }"
	                       "{ \"long\", 999 }";
	int         pipe_fd[2];
	char        buff[4096] = {0};
	int         n;

	assert(pipe((int *)&pipe_fd) >= 0);
	assert(ctls_dump_typewrap(vars[0], pipe_fd[1]) == true);
	assert(ctls_dump_typewrap(vars[1], pipe_fd[1]) == true);
	assert(ctls_dump_typewrap(vars[2], pipe_fd[1]) == true);
	assert(ctls_dump_typewrap(vars[3], pipe_fd[1]) == true);
	assert(ctls_dump_typewrap(vars[4], pipe_fd[1]) == true);
	assert(ctls_dump_typewrap(vars[5], pipe_fd[1]) == true);
	assert(ctls_dump_typewrap((t_ctls_typewrap){0}, -1) == false);
	assert(ctls_dump_typewrap(vars[1], -1) == false);
	close(pipe_fd[1]);
	n = read(pipe_fd[0], buff, sizeof(buff) / sizeof(buff[0]));
	close(pipe_fd[0]);
	assert(n >= 0);
	assert(!strcmp(buff, expected));
}
