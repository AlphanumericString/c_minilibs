#include <assert.h>
#include <string.h>
#include <unistd.h>

#include "sptr_tests.h"

// set dtor at 0x42 for test instead of using real func (eg:free) as free is
// loaded at random mem space.
void test_sptr_dumps(void) {
	t_strong_ptr p1 = sptr_make_shared((void *)0xDEAD, NULL);
	t_strong_ptr p2 = sptr_make_shared((void *)0xBEEF, (void *)0x42);
	t_strong_ptr p3 = sptr_make_shared((void *)0xFACE, NULL);
	t_weak_ptr   w1 = sptr_make_shared(p3, NULL);
	const char  *expected_str =
	    "SPTR STRONG { data 0xdead, count 1 (weak:0), dtor (nil)}"
	    "SPTR STRONG { data 0xbeef, count 1 (weak:0), dtor 0x42}"
	    "SPTR STRONG { data 0xface, count 1 (weak:1), dtor (nil)}";

	int  pfd[2]    = {0};
	char buf[4096] = {0};

	if (pipe(pfd) == -1) assert(false);
	sptr_dumps(pfd[1], p1);
	sptr_dumps(pfd[1], p2);
	sptr_dumps(pfd[1], p3);
	close(pfd[1]);
	read(pfd[0], buf, sizeof(buf));
	close(pfd[0]);
	assert(strcmp(buf, expected_str) == 0);
	p2->dtor = NULL; // re-set to null as free 0x42 is not a valid fptr
	sptr_delete(p1);
	sptr_delete(p2);
	sptr_delete(p3);
	sptr_delete(w1);
	return;
}
void test_sptr_dumpw(void) {
	t_strong_ptr s1           = sptr_make_shared((void *)0xDEAD, NULL);
	t_strong_ptr s2           = sptr_make_shared((void *)0xBEEF, (void *)0x42);
	t_strong_ptr s3           = sptr_make_shared((void *)0xCAFE, NULL);
	t_weak_ptr   w1           = sptr_make_shared(s1, NULL);
	t_weak_ptr   w2           = sptr_make_shared(s2, NULL);
	t_weak_ptr   w2_secondary = sptr_copy(w2);
	t_weak_ptr   w3           = sptr_make_shared(s3, NULL);
	const char  *expected_str =
	    "SPTR WEAK { data 0xdead, count 1 (weak:1), dtor (nil)}"
	    "SPTR WEAK { data 0xbeef, count 1 (weak:2), dtor 0x42}"
	    "SPTR WEAK { data (nil), count 0 (weak:1), dtor (nil)}";
	int  pfd[2]    = {0};
	char buf[4096] = {0};

	if (pipe(pfd) == -1) assert(false);
	sptr_delete(s3);
	sptr_dumpw(pfd[1], w1);
	sptr_dumpw(pfd[1], w2);
	sptr_dumpw(pfd[1], w3);
	close(pfd[1]);
	read(pfd[0], buf, sizeof(buf));
	close(pfd[0]);
	assert(strcmp(buf, expected_str) == 0);
	s2->dtor = NULL; // re-set to null as free 0x42 is not a valid fptr
	sptr_delete(s1);
	sptr_delete(s2);
	sptr_delete(w1);
	sptr_delete(w2);
	sptr_delete(w2_secondary);
	sptr_delete(w3);
	return;
}
void test_sptr_dumpu(void) {
	t_uniq_ptr  u1 = sptr_make_uniq((void *)0xDEAD, NULL);
	t_uniq_ptr  u2 = sptr_make_uniq((void *)0xBEEF, (void *)0x42);
	t_uniq_ptr  u3 = sptr_make_uniq(u2, NULL);
	const char *expected_str =
	    "SPTR UNIQ owned { data 0xdead, count 1 (weak:0), dtor (nil)}"
	    "SPTR UNIQ owned { data 0xbeef, count 2 (weak:1), dtor 0x42}"
	    "SPTR UNIQ borrowed { data 0xbeef, count 2 (weak:1), dtor 0x42}";
	int  pfd[2]    = {0};
	char buf[4096] = {0};

	if (pipe(pfd) == -1) assert(false);
	sptr_dumpu(pfd[1], u1);
	sptr_dumpu(pfd[1], u2);
	sptr_dumpu(pfd[1], u3);
	close(pfd[1]);
	read(pfd[0], buf, sizeof(buf));
	close(pfd[0]);
	assert(strcmp(buf, expected_str) == 0);
	u2.rcbox->dtor = NULL; // re-set to null as free 0x42 is not a valid fptr
	sptr_delete(u1);
	sptr_delete(u2);
	sptr_delete(u3);
	return;
}
