#include <assert.h>
#include <unistd.h>

#include "tls_tests.h"

NO_COV void test_ctls_dump_all(void) {
	const char *expected = "a:{ \"int\", -1 }\n"
	                       "b:{ \"char\", d (100) }\n"
	                       "c:{ \"unknown\" }\n"
	                       "d:{ \"ptr\", 0xdeadbeef }\n"
	                       "e:{ \"ptr\", 0x42 }\n"
	                       "f:{ \"long\", 999 }\n";

	const t_cm_ctlsv empty_ctls = (t_cm_ctlsv){0};
	const t_cm_ctlsv clts       = (t_cm_ctlsv){
	          .len = 6,
	          .vals =
            {
                   {CTLS_INT, .value.i = -1},
                   {CTLS_CHAR, .value.c = 'd'},
                   {99, .value.p = (void *)0xDEAD},
                   {CTLS_PTR, .value.l = (long)0xDEADBEEF},
                   {CTLS_PTR, .value.i = (int)0x42},
                   {CTLS_LONG, .value.l = (long)999},
	               },
	          .v_name = {"a", "b", "c", "d", "e", "f"}
    };
	t_cm_ctlsv *prev;
	int         pipe_fd[2];
	char        buff[6096] = {0};
	int         n;

	prev = pthread_getspecific(ctls_key);

	assert(pipe((int *)&pipe_fd) >= 0);
	pthread_setspecific(ctls_key, &clts);
	assert(ctls_dump_all(pipe_fd[1]) == clts.len);
	assert(ctls_dump_all(-1) == -1);
	assert(ctls_dump_all(-99) == -1);
	pthread_setspecific(ctls_key, NULL);
	assert(ctls_dump_all(pipe_fd[1]) == 0);
	pthread_setspecific(ctls_key, &empty_ctls);
	assert(ctls_dump_all(pipe_fd[1]) == 0);

	close(pipe_fd[1]);
	n = read(pipe_fd[0], buff, sizeof(buff) / sizeof(buff[0]));
	close(pipe_fd[0]);

	pthread_setspecific(ctls_key, prev);

	assert(n >= 0);
	assert(!strcmp(buff, expected));
}

NO_COV void test_ctls_dump_typewrap(void) {
	const t_cm_ctls_typewrap vars[] = {
	    (t_cm_ctls_typewrap){.type = CTLS_INT,  .value.i = -1              },
	    (t_cm_ctls_typewrap){.type = CTLS_CHAR, .value.c = 'd'             },
	    (t_cm_ctls_typewrap){.type = 99,        .value.p = (void *)0xDEAD  },
	    (t_cm_ctls_typewrap){.type = CTLS_PTR,  .value.l = (long)0xDEADBEEF},
	    (t_cm_ctls_typewrap){.type = CTLS_PTR,  .value.i = (int)0x42       },
	    (t_cm_ctls_typewrap){.type = CTLS_LONG, .value.l = (long)999       },
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
	assert(ctls_dump_typewrap((t_cm_ctls_typewrap){0}, -1) == false);
	assert(ctls_dump_typewrap(vars[1], -1) == false);
	close(pipe_fd[1]);
	n = read(pipe_fd[0], buff, sizeof(buff) / sizeof(buff[0]));
	close(pipe_fd[0]);
	assert(n >= 0);
	assert(!strcmp(buff, expected));
}
