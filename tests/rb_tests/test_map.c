#include <assert.h>
#include <stdlib.h>

#include "rb_tests.h"

NO_COV void test_cm_rb_map(void) {
	t_cm_rb rb     = {0};
	int     arr[3] = {1, 2, 3};
	int    *out[3] = {0};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 3);

	assert(rb_map(&rb, cb_dup, (void **)out) == true);

	assert(out[0] != NULL && out[1] != NULL && out[2] != NULL);
	assert(*out[0] == 1 && *out[1] == 2 && *out[2] == 3);
	free(out[0]);
	free(out[1]);
	free(out[2]);

	assert(rb_map(NULL, cb_dup, (void **)out) == false);
	assert(rb_map(&rb, NULL, (void **)out) == false);
	assert(rb_map(&rb, cb_dup, NULL) == false);
	rb_destroy(&rb);
}

NO_COV void test_cm_rb_nmap(void) {
	t_cm_rb rb     = {0};
	int     arr[4] = {1, 2, 3, 4};
	int    *out[4] = {0};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 4);

	assert(rb_nmap(&rb, 2, cb_dup, (void **)out) == true);
	assert(out[0] != NULL && out[1] != NULL);
	assert(*out[0] == 1);
	assert(*out[1] == 2);
	free(out[0]);
	free(out[1]);
	assert(rb_nmap(&rb, 2, cb_dup_fail3, (void **)out) == true);
	assert(out[0] != NULL && out[1] != NULL);
	assert(*out[0] == 1);
	assert(*out[1] == 2);
	free(out[0]);
	free(out[1]);
	assert(rb_nmap(&rb, -1, cb_dup, (void **)out) == true);
	assert(out[0] != NULL && out[1] != NULL && out[2] != NULL &&
	       out[3] != NULL);
	assert(*out[0] == 1);
	assert(*out[1] == 2);
	assert(*out[2] == 3);
	assert(*out[3] == 4);
	free(out[0]);
	free(out[1]);
	free(out[2]);
	free(out[3]);

	out[3] = (void *)0xDEADBEEF;
	assert(rb_nmap(&rb, -1, cb_dup_fail3, (void **)out) == false);
	assert(out[0] != NULL && out[1] != NULL && out[2] == NULL &&
	       out[3] != NULL && out[3] == (void *)0xDEADBEEF);
	assert(*out[0] == 1);
	assert(*out[1] == 2);
	free(out[0]);
	free(out[1]);

	assert(rb_nmap(NULL, 2, cb_dup, (void **)out) == false);
	assert(rb_nmap(&rb, 0, cb_dup, (void **)out) == false);
	assert(rb_nmap(&rb, 2, NULL, (void **)out) == false);
	assert(rb_nmap(&rb, 2, cb_dup, NULL) == false);
	rb_destroy(&rb);
}
