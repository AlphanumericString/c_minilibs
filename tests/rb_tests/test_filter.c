#include <assert.h>

#include "rb_tests.h"

static bool cb_is_odd(void *elem, void *arg) {
	(void)arg;
	return (*(int *)elem % 2) == 1;
}
static bool cb_is_even(void *elem, void *arg) {
	(void)arg;
	return (*(int *)elem % 2) == 0;
}

NO_COV void test_cm_rb_filter(void) {
	t_cm_rb rb     = {0};
	int     arr[4] = {1, 2, 3, 4};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 4);

	assert(rb_size(rb) == 4);
	assert(rb_filter(&rb, cb_is_even, NULL, cb_destroy) == true);
	assert(rb_size(rb) == 2);
	assert(rb_at(rb, 0) != NULL && rb_at(rb, 1) != NULL);
	assert(*(int *)rb_at(rb, 0) == 2);
	assert(*(int *)rb_at(rb, 1) == 4);

	rb_clr(&rb);
	rb_npush(&rb, arr, 4);

	// check other filter + null destroyer doesnt brick funct
	assert(rb_size(rb) == 4);
	assert(rb_filter(&rb, cb_is_odd, NULL, NULL) == true);
	assert(rb_size(rb) == 2);
	assert(rb_at(rb, 0) != NULL && rb_at(rb, 1) != NULL);
	assert(*(int *)rb_at(rb, 0) == 1);
	assert(*(int *)rb_at(rb, 1) == 3);

	assert(rb_filter(NULL, cb_is_even, NULL, cb_destroy) == false);
	assert(rb_filter(&rb, NULL, NULL, cb_destroy) == false);

	rb_destroy(&rb);
}
