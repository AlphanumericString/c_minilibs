#include <assert.h>
#include <unistd.h>

#include "tls_tests.h"

NO_COV void test_vtls_dump(void) {
	void       *d1       = (void *)0xDEAD;
	void       *d2       = (void *)0x42;
	void       *d3       = (void *)0xBEEF;
	void       *d4       = (void *)0xDEADCAFEBEEFBEAD;
	void       *d5       = (void *)0x0;
	const char *expected = "a:{           0xdead 3 }\n"
	                       "b:{             0x42 12 }\n"
	                       "c:{           0xbeef 1 }\n"
	                       "d:{           0xbeef 0 }\n"
	                       "f:{            (nil) 3 }\n";
	t_vtls_key  keys[]   = {
        {.v_name = "a",  .var = d1, .size_e = 3 },
        {.v_name = "b",  .var = d2, .size_e = 12},
        {.v_name = "c",  .var = d3, .size_e = 1 },
        {.v_name = "d",  .var = d3, .size_e = 0 },
        {.v_name = NULL, .var = d4, .size_e = 3 },
        {.v_name = "f",  .var = d5, .size_e = 3 },
    };
	const t_vtls  empty_vtls = (t_vtls){0};
	const t_vtls  vtls       = (t_vtls){.len = 6, .cap = 12, .data = keys};
	const t_vtls *prev       = pthread_getspecific(vtls_key);
	int           pipe_fd[2];
	char          buff[4096] = {0};
	int           n;

	assert(pipe((int *)&pipe_fd) >= 0);
	pthread_setspecific(vtls_key, &vtls);
	assert(vtls_dump("a", pipe_fd[1]) == true);
	assert(vtls_dump("b", pipe_fd[1]) == true);
	assert(vtls_dump("c", pipe_fd[1]) == true);
	assert(vtls_dump("d", pipe_fd[1]) == true);
	assert(vtls_dump("f", pipe_fd[1]) == true);
	assert(vtls_dump(NULL, pipe_fd[1]) == false);
	assert(vtls_dump("whatever", pipe_fd[1]) == false);
	assert(vtls_dump("a", -1) == false); // bad fds
	assert(vtls_dump("whatever", -1) == false);
	assert(vtls_dump("a", -99) == false);
	pthread_setspecific(vtls_key, NULL);
	assert(vtls_dump("a", pipe_fd[1]) == false); // null vtls
	assert(vtls_dump("whatever", pipe_fd[1]) == false);
	pthread_setspecific(vtls_key, &empty_vtls);
	assert(vtls_dump("a", pipe_fd[1]) == false); // empty vtls
	assert(vtls_dump("whatever", pipe_fd[1]) == false);

	close(pipe_fd[1]);
	n = read(pipe_fd[0], buff, sizeof(buff) / sizeof(buff[0]));
	close(pipe_fd[0]);

	pthread_setspecific(vtls_key, prev);

	assert(n >= 0);
	assert(!strcmp(buff, expected));
}
NO_COV void test_vtls_dump_all(void) {
	void      *d1     = (void *)0xDEAD;
	void      *d2     = (void *)0x42;
	void      *d3     = (void *)0xBEEF;
	void      *d4     = (void *)0xDEADCAFEBEEFBEAD;
	void      *d5     = (void *)0x0;
	t_vtls_key keys[] = {
	    {.v_name = "a",  .var = d1, .size_e = 3 },
	    {.v_name = "b",  .var = d2, .size_e = 12},
	    {.v_name = "c",  .var = d3, .size_e = 1 },
	    {.v_name = "d",  .var = d3, .size_e = 0 },
	    {.v_name = NULL, .var = d4, .size_e = 3 },
	    {.v_name = "f",  .var = d5, .size_e = 3 },
	};
	const char   *expected   = "a:{           0xdead 3 }\n"
	                           "b:{             0x42 12 }\n"
	                           "c:{           0xbeef 1 }\n"
	                           "d:{           0xbeef 0 }\n"
	                           "(null):{ 0xdeadcafebeefbead 3 }\n"
	                           "f:{            (nil) 3 }\n";
	const t_vtls  empty_vtls = (t_vtls){0};
	const t_vtls  vtls       = (t_vtls){.len = 6, .cap = 12, .data = keys};
	const t_vtls *prev       = pthread_getspecific(vtls_key);
	int           pipe_fd[2];
	char          buff[4096] = {0};
	int           n;

	assert(pipe((int *)&pipe_fd) >= 0);
	pthread_setspecific(vtls_key, &vtls);
	assert(vtls_dump_all(pipe_fd[1]) == (ssize_t)vtls.len);
	assert(vtls_dump_all(-1) == -1);
	assert(vtls_dump_all(-99) == -1);
	pthread_setspecific(vtls_key, NULL);
	assert(vtls_dump_all(pipe_fd[1]) == 0);
	pthread_setspecific(vtls_key, &empty_vtls);
	assert(vtls_dump_all(pipe_fd[1]) == 0);

	close(pipe_fd[1]);
	n = read(pipe_fd[0], buff, sizeof(buff) / sizeof(buff[0]));
	close(pipe_fd[0]);

	pthread_setspecific(vtls_key, prev);

	assert(n >= 0);
	assert(!strcmp(buff, expected));
}
