#include <assert.h>
#include <unistd.h>

#include "tls_tests.h"

#define T1_STR "{ \"int\", -1 }"
#define T2_STR "{ \"char\", d (100) }"
#define T3_STR "{ \"unknown\" }"
#define T4_STR "{ \"ptr\", 0xdeadbeef }"

void test_ctls_dump_typewrap(void) {
	const t_ctls_typewrap t1 =
	    (t_ctls_typewrap){.type = CTLS_INT, .value.i = -1};
	const t_ctls_typewrap t2 =
	    (t_ctls_typewrap){.type = CTLS_CHAR, .value.c = 'd'};
	const t_ctls_typewrap t3 =
	    (t_ctls_typewrap){.type = 12, .value.p = (void *)3012};
	const t_ctls_typewrap t4 =
	    (t_ctls_typewrap){.type = CTLS_PTR, .value.p = (void *)0xDEADBEEF};
	int  pipe_fd[2];
	char buff[4096] = {0};
	int  n;

	assert(pipe((int *)&pipe_fd) >= 0);
	ctls_dump_typewrap(t1, pipe_fd[1]);
	ctls_dump_typewrap(t2, pipe_fd[1]);
	ctls_dump_typewrap(t3, pipe_fd[1]);
	ctls_dump_typewrap(t4, pipe_fd[1]);
	close(pipe_fd[1]);
	n = read(pipe_fd[0], buff, sizeof(buff) / sizeof(buff[0]));
	close(pipe_fd[0]);
	assert(n >= 0);
	assert(!strcmp(buff, T1_STR T2_STR T3_STR T4_STR));
}
