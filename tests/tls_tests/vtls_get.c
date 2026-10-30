#include <assert.h>

#include "tls_tests.h"

NO_COV void test_vtls_get(void) {
	void *data;
	assert(vtls_get(NULL) == NULL);
	assert(vtls_get("a") == NULL);
	vtls_set("a", (void *)0xDEAD, sizeof(void *));
	assert(vtls_get("a") != NULL);
	assert(vtls_get("a") == (void *)0xDEAD);

	data = pthread_getspecific(vtls_key);
	assert(data != NULL);                             // was already init-ed
	assert(pthread_setspecific(vtls_key, NULL) == 0); // set to non-init val
	assert(vtls_get("a") == NULL);                    // fails (no vtls)
	assert(pthread_getspecific(vtls_key) == NULL);    // func hasn't init vtls
	assert(pthread_setspecific(vtls_key, data) == 0); // reset

	vtls_rm("a");
}
NO_COV void test_vtls_sizeof(void) {
	void *prev = pthread_getspecific(vtls_key);

	vtls_set("foo", (void *)0x42, 42);
	vtls_set("foo1", (void *)0x42, -1);
	vtls_set("foo2", (void *)0x42, 13);
	assert(vtls_sizeof("foo") == 42);
	assert(vtls_sizeof("foo1") == (size_t)-1);
	assert(vtls_sizeof("foo2") == 13);
	assert(vtls_sizeof("whatever") == 0);
	assert(vtls_sizeof(NULL) == 0);
	pthread_setspecific(vtls_key, NULL);
	assert(vtls_sizeof("foo") == 0);
	assert(vtls_sizeof("foo1") == 0);
	assert(vtls_sizeof("foo2") == 0);
	assert(vtls_sizeof("whatever") == 0);
	pthread_setspecific(vtls_key, prev);
	vtls_rm("foo");
	vtls_rm("foo1");
	vtls_rm("foo2");
}
NO_COV void test_vtls_getout(void) {
	void *prev = pthread_getspecific(vtls_key);
	char  out[42];
	int   v0 = 42;
	char  v1 = 'x';
	long  v2 = 0xDEAD;
	struct s_custom_len_obj {
		int   t;
		long  d;
		void *f;
	} v3 = (struct s_custom_len_obj){.t = 32, .d = -1, .f = (void *)0xBEAD};

	vtls_set("key0", &v0, sizeof(v0));
	vtls_set("key1", &v1, sizeof(v1));
	vtls_set("key2", &v2, sizeof(v2));
	vtls_set("key3", &v3, sizeof(v3));
	assert(vtls_getout("key0", &out) == true);
	assert(memcmp(out, &v0, sizeof(v0)) == 0);
	assert(vtls_getout("key1", &out) == true);
	assert(memcmp(out, &v1, sizeof(v1)) == 0);
	assert(vtls_getout("key2", &out) == true);
	assert(memcmp(out, &v2, sizeof(v2)) == 0);
	assert(vtls_getout("key3", &out) == true);
	assert(memcmp(out, &v3, sizeof(v3)) == 0);
	assert(vtls_getout("whatever", &out) == false);
	assert(vtls_getout(NULL, &out) == false);
	pthread_setspecific(vtls_key, NULL);
	assert(vtls_getout("key0", &out) == false);
	assert(vtls_getout("key1", &out) == false);
	assert(vtls_getout("key2", &out) == false);
	assert(vtls_getout("key3", &out) == false);
	assert(vtls_getout("whatever", &out) == false);
	assert(vtls_getout(NULL, &out) == false);
	pthread_setspecific(vtls_key, prev);
	vtls_rm("key0");
	vtls_rm("key1");
	vtls_rm("key2");
	vtls_rm("key3");
}

NO_COV void test_vtls_len(void) {
	void *data;

	assert(vtls_len() == 0);
	vtls_set("a", (void *)0xDEAD, sizeof(void *));
	assert(vtls_len() == 1);
	vtls_set("b", (void *)0xDEAD, sizeof(void *));
	assert(vtls_len() == 2);

	data = pthread_getspecific(vtls_key);
	assert(data != NULL);                             // was already init-ed
	assert(pthread_setspecific(vtls_key, NULL) == 0); // set to non-init val
	assert(vtls_len() == 0);                          // 0 (no vtls)
	assert(pthread_getspecific(vtls_key) == NULL);    // func hasn't init vtls
	assert(pthread_setspecific(vtls_key, data) == 0); // reset

	vtls_rm("a");
	vtls_rm("b");
	return;
}
