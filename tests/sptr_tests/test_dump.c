#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sptr_tests.h"

NO_COV void test_sptr_to_string_s(void) {
	const t_cm_fbox b1 = (t_cm_fbox){(void *)0xDEAD, NULL, 1, 0};
	const t_cm_fbox b2 = (t_cm_fbox){(void *)0xBEEF, (t_cm_dtor)0x42, 1, 0};
	const t_cm_fbox b3 = (t_cm_fbox){(void *)0xFACE, NULL, 1, 1};
	const t_cm_strong_ptr s[] = {
	    (const t_cm_strong_ptr){b1.data, (t_cm_fbox *)&b1},
	    (const t_cm_strong_ptr){b2.data, (t_cm_fbox *)&b2},
	    (const t_cm_strong_ptr){b3.data, (t_cm_fbox *)&b3},
	    (const t_cm_strong_ptr){NULL,    NULL            }
    };
	const char *e[sizeof(s) / sizeof(s[0]) + 1] = {
	    "SPTR STRONG { data 0xdead, count 1 (weak:0), dtor (nil)}",
	    "SPTR STRONG { data 0xbeef, count 1 (weak:0), dtor 0x42}",
	    "SPTR STRONG { data 0xface, count 1 (weak:1), dtor (nil)}",
	    "SPTR STRONG (nil)", "SPTR STRONG (nil)"};
	char *r[sizeof(s) / sizeof(s[0]) + 1] = {
	    sptr_to_strings(&s[0]), sptr_to_strings(&s[1]), sptr_to_strings(&s[2]),
	    sptr_to_strings(&s[3]), sptr_to_strings(NULL)};

	assert(strcmp(r[0], e[0]) == 0);
	assert(strcmp(r[1], e[1]) == 0);
	assert(strcmp(r[2], e[2]) == 0);
	assert(strcmp(r[3], e[3]) == 0);
	assert(strcmp(r[4], e[4]) == 0);
	free(r[0]);
	free(r[1]);
	free(r[2]);
	free(r[3]);
	free(r[4]);

	alloc_set_call_limit(0);
	assert(sptr_to_strings(&s[0]) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
}
NO_COV void test_sptr_to_string_w(void) {
	const t_cm_fbox f1 = (t_cm_fbox){(void *)0xDEAD, NULL, 1, 1};
	const t_cm_fbox f2 = (t_cm_fbox){(void *)0xBEEF, (t_cm_dtor)0x42, 1, 2};
	const t_cm_fbox f3 = (t_cm_fbox){NULL, NULL, 0, 1};

	const t_cm_weak_ptr w[] = {(const t_cm_weak_ptr){(t_cm_fbox *)&f1},
	                           (const t_cm_weak_ptr){(t_cm_fbox *)&f2},
	                           (const t_cm_weak_ptr){(t_cm_fbox *)&f3},
	                           (const t_cm_weak_ptr){NULL}};
	const char         *e_str[sizeof(w) / sizeof(w[0])] = {
        "SPTR WEAK { data 0xdead, count 1 (weak:1), dtor (nil)}",
        "SPTR WEAK { data 0xbeef, count 1 (weak:2), dtor 0x42}",
        "SPTR WEAK { data (nil), count 0 (weak:1), dtor (nil)}",
        "SPTR WEAK (nil)"};
	char *res[sizeof(w) / sizeof(w[0])] = {
	    sptr_to_stringw(&w[0]),
	    sptr_to_stringw(&w[1]),
	    sptr_to_stringw(&w[2]),
	    sptr_to_stringw(&w[3]),
	};

	assert(strcmp(res[0], e_str[0]) == 0);
	assert(strcmp(res[1], e_str[1]) == 0);
	assert(strcmp(res[2], e_str[2]) == 0);
	assert(strcmp(res[3], e_str[3]) == 0);
	free(res[0]);
	free(res[1]);
	free(res[2]);
	free(res[3]);

	alloc_set_call_limit(0);
	assert(sptr_to_stringw(&w[0]) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
}
NO_COV void test_sptr_to_string_o(void) {
	t_cm_fbox            f1  = {(void *)0xDEAD, NULL, 1, 0};
	t_cm_fbox            f2  = {(void *)0xBEEF, (t_cm_dtor)0x42, 1, 1};
	const t_cm_owned_ptr o[] = {
	    (t_cm_owned_ptr){.owner = true,  .meta = &f1,  .data = f1.data},
	    (t_cm_owned_ptr){.owner = true,  .meta = &f2,  .data = f2.data},
	    (t_cm_owned_ptr){.owner = false, .meta = &f2,  .data = f2.data},
	    (t_cm_owned_ptr){.owner = true,  .meta = NULL, .data = NULL   }
    };
	const char *e[sizeof(o) / sizeof(o[0]) + 1] = {
	    "SPTR OWND owned { data 0xdead, count 1 (weak:0), dtor (nil)}",
	    "SPTR OWND owned { data 0xbeef, count 1 (weak:1), dtor 0x42}",
	    "SPTR OWND borrowed { data 0xbeef, count 1 (weak:1), dtor 0x42}",
	    "SPTR OWND (nil)", "SPTR OWND (nil)"};
	char *r[sizeof(o) / sizeof(o[0]) + 1] = {
	    sptr_to_stringo(&o[0]), sptr_to_stringo(&o[1]), sptr_to_stringo(&o[2]),
	    sptr_to_stringo(&o[3]), sptr_to_stringo(NULL)};

	assert(strcmp(r[0], e[0]) == 0);
	assert(strcmp(r[1], e[1]) == 0);
	assert(strcmp(r[2], e[2]) == 0);
	assert(strcmp(r[3], e[3]) == 0);
	assert(strcmp(r[4], e[4]) == 0);
	free(r[0]);
	free(r[1]);
	free(r[2]);
	free(r[3]);
	free(r[4]);

	alloc_set_call_limit(0);
	assert(sptr_to_stringo(&o[0]) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
}
NO_COV void test_sptr_to_string_u(void) {
	const t_cm_uniq_ptr u[] = {
	    (t_cm_uniq_ptr){(void *)0xDEAD, NULL           },
	    (t_cm_uniq_ptr){(void *)0xBEEF, (t_cm_dtor)0x42}
    };
	const char *e[sizeof(u) / sizeof(u[0]) + 1] = {
	    "SPTR UNIQ { data 0xdead, dtor (nil)}",
	    "SPTR UNIQ { data 0xbeef, dtor 0x42}", "SPTR UNIQ (nil)"};
	char *r[sizeof(u) / sizeof(u[0]) + 1] = {
	    sptr_to_stringu(&u[0]), sptr_to_stringu(&u[1]), sptr_to_stringu(NULL)};

	assert(strcmp(r[0], e[0]) == 0);
	assert(strcmp(r[1], e[1]) == 0);
	assert(strcmp(r[2], e[2]) == 0);
	free(r[0]);
	free(r[1]);
	free(r[2]);

	alloc_set_call_limit(0);
	assert(sptr_to_stringu(&u[0]) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
}

// set dtor at 0x42 for test instead of using real func (eg:free) as free is
// loaded at random mem space.
NO_COV void test_sptr_dumps(void) {
	const t_cm_fbox b1 = (t_cm_fbox){(void *)0xDEAD, (t_cm_dtor)NULL, 1, 0};
	const t_cm_strong_ptr s =
	    (const t_cm_strong_ptr){b1.data, (t_cm_fbox *)&b1};
	const char *s_str =
	    "SPTR STRONG { data 0xdead, count 1 (weak:0), dtor (nil)}\n";
	const char *null_str = "SPTR STRONG (nil)\n";
	int         pfd[2];
	char        buf[256] = {0};

	assert(sptr_dumps(-1, &s) == false);
	assert(sptr_dumps(-1, NULL) == false);

	assert(pipe(pfd) == 0);
	assert(sptr_dumps(pfd[1], &s) == true);
	ssize_t n = read(pfd[0], buf, sizeof(buf) - 1);
	assert(n > 0);
	buf[n] = '\0';
	assert(strcmp(buf, s_str) == 0);
	close(pfd[0]);
	close(pfd[1]);

	assert(pipe(pfd) == 0);
	assert(sptr_dumps(pfd[1], NULL) == true);
	memset(buf, 0, sizeof(buf));
	n = read(pfd[0], buf, sizeof(buf) - 1);
	assert(n > 0);
	buf[n] = '\0';
	assert(strcmp(buf, null_str) == 0);
	close(pfd[0]);
	close(pfd[1]);

	assert(pipe(pfd) == 0);
	alloc_set_call_limit(0);
	assert(sptr_dumps(pfd[1], &s) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	close(pfd[0]);
	close(pfd[1]);
}

NO_COV void test_sptr_dumpw(void) {
	const t_cm_fbox     f1 = (t_cm_fbox){(void *)0xDEAD, NULL, 1, 1};
	const t_cm_weak_ptr w  = (const t_cm_weak_ptr){(t_cm_fbox *)&f1};
	const char         *w_str =
	    "SPTR WEAK { data 0xdead, count 1 (weak:1), dtor (nil)}\n";
	const char *null_str = "SPTR WEAK (nil)\n";
	int         pfd[2];
	char        buf[256] = {0};

	sptr_dumpw(-1, &w);
	sptr_dumpw(-1, NULL);

	assert(pipe(pfd) == 0);
	sptr_dumpw(pfd[1], &w);
	ssize_t n = read(pfd[0], buf, sizeof(buf) - 1);
	assert(n > 0);
	buf[n] = '\0';
	assert(strcmp(buf, w_str) == 0);
	close(pfd[0]);
	close(pfd[1]);

	assert(pipe(pfd) == 0);
	sptr_dumpw(pfd[1], NULL);
	memset(buf, 0, sizeof(buf));
	n = read(pfd[0], buf, sizeof(buf) - 1);
	assert(n > 0);
	buf[n] = '\0';
	assert(strcmp(buf, null_str) == 0);
	close(pfd[0]);
	close(pfd[1]);

	assert(pipe(pfd) == 0);
	alloc_set_call_limit(0);
	assert(sptr_dumpw(pfd[1], &w) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	close(pfd[0]);
	close(pfd[1]);
}

NO_COV void test_sptr_dumpo(void) {
	t_cm_fbox            f1 = {(void *)0xDEAD, NULL, 1, 0};
	const t_cm_owned_ptr o =
	    (t_cm_owned_ptr){.owner = true, .meta = &f1, .data = f1.data};
	const char *o_str =
	    "SPTR OWND owned { data 0xdead, count 1 (weak:0), dtor (nil)}\n";
	const char *null_str = "SPTR OWND (nil)\n";
	int         pfd[2];
	char        buf[256] = {0};

	sptr_dumpo(-1, &o);
	sptr_dumpo(-1, NULL);

	assert(pipe(pfd) == 0);
	sptr_dumpo(pfd[1], &o);
	ssize_t n = read(pfd[0], buf, sizeof(buf) - 1);
	assert(n > 0);
	buf[n] = '\0';
	assert(strcmp(buf, o_str) == 0);
	close(pfd[0]);
	close(pfd[1]);

	assert(pipe(pfd) == 0);
	sptr_dumpo(pfd[1], NULL);
	memset(buf, 0, sizeof(buf));
	n = read(pfd[0], buf, sizeof(buf) - 1);
	assert(n > 0);
	buf[n] = '\0';
	assert(strcmp(buf, null_str) == 0);
	close(pfd[0]);
	close(pfd[1]);

	assert(pipe(pfd) == 0);
	alloc_set_call_limit(0);
	assert(sptr_dumpo(pfd[1], &o) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	close(pfd[0]);
	close(pfd[1]);
}

NO_COV void test_sptr_dumpu(void) {
	const t_cm_uniq_ptr u        = (t_cm_uniq_ptr){(void *)0xDEAD, NULL};
	const char         *u_str    = "SPTR UNIQ { data 0xdead, dtor (nil)}\n";
	const char         *null_str = "SPTR UNIQ (nil)\n";
	int                 pfd[2];
	char                buf[256] = {0};

	sptr_dumpu(-1, &u);
	sptr_dumpu(-1, NULL);

	assert(pipe(pfd) == 0);
	sptr_dumpu(pfd[1], &u);
	ssize_t n = read(pfd[0], buf, sizeof(buf) - 1);
	assert(n > 0);
	buf[n] = '\0';
	assert(strcmp(buf, u_str) == 0);
	close(pfd[0]);
	close(pfd[1]);

	assert(pipe(pfd) == 0);
	sptr_dumpu(pfd[1], NULL);
	memset(buf, 0, sizeof(buf));
	n = read(pfd[0], buf, sizeof(buf) - 1);
	assert(n > 0);
	buf[n] = '\0';
	assert(strcmp(buf, null_str) == 0);
	close(pfd[0]);
	close(pfd[1]);

	assert(pipe(pfd) == 0);
	alloc_set_call_limit(0);
	assert(sptr_dumpu(pfd[1], &u) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	close(pfd[0]);
	close(pfd[1]);
}
