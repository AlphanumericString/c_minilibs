#include "rb_tests.h"

static bool cb_is_even(void *elem, void *arg) {
	(void)arg;
	return (*(int *)elem % 2) == 0;
}

void test_rb_filter(void) {
	t_rb rb     = {0};
	int  arr[4] = {1, 2, 3, 4};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 4);

	assert(rb_filter(&rb, cb_is_even, NULL, cb_destroy) == true);
	assert(rb_size(rb) == 2);

	rb_destroy(&rb);
}
