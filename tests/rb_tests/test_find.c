#include "rb_tests.h"

void test_rb_find(void) {
	t_rb rb     = {0};
	int  arr[4] = {1, 2, 3, 4};
	int  target = 3;

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 4);

	int *p = rb_find(&rb, cb_cmp, &target);

	assert(p != NULL);
	assert(*p == 3);

	rb_destroy(&rb);
}
