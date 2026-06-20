#include "rb_tests.h"

void test_rb_get(void) {
	t_rb rb     = {0};
	int  arr[4] = {1, 2, 3, 4};
	int  target = 3;
	int  out    = 0;

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 4);

	assert(rb_get(&rb, cb_cmp, &target, &out) != NULL);
	assert(out == 3);

	rb_destroy(&rb);
}

void test_rb_nget(void) {
	t_rb rb     = {0};
	int  arr[4] = {1, 2, 3, 4};
	int  target = 2;
	int  out[2] = {0};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 4);

	assert(rb_nget(&rb, 2, cb_cmp, &target, out) != NULL);

	rb_destroy(&rb);
}
