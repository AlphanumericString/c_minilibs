#include "rb_tests.h"

void test_rb_map(void) {
	t_rb rb     = {0};
	int  arr[3] = {1, 2, 3};
	int *out[3] = {0};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 3);

	assert(rb_map(&rb, cb_dup, out) == true);
	assert(out[0] != NULL);
	assert(*out[0] == 1);
	assert(out[2] != NULL);
	assert(*out[2] == 3);

	free(out[0]);
	free(out[1]);
	free(out[2]);
	rb_destroy(&rb);
}

void test_rb_nmap(void) {
	t_rb rb     = {0};
	int  arr[3] = {1, 2, 3};
	int *out[2] = {0};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 3);

	assert(rb_nmap(&rb, 2, cb_dup, out) == true);
	assert(out[0] != NULL);
	assert(*out[0] == 1);
	assert(out[1] != NULL);
	assert(*out[1] == 2);

	free(out[0]);
	free(out[1]);
	rb_destroy(&rb);
}
