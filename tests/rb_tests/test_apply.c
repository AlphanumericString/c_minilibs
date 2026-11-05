#include <assert.h>

#include "rb_tests.h"

NO_COV void test_cm_rb_apply(void) {
	t_cm_rb rb     = {0};
	int     arr[3] = {1, 2, 3};

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 3);

	assert(rb_apply(&rb, cb_add_one, NULL) == true);
	assert(*(int *)rb_at(rb, 0) == 2);
	assert(*(int *)rb_at(rb, 1) == 3);
	assert(*(int *)rb_at(rb, 2) == 4);
	assert(rb_apply(&rb, NULL, NULL) == false);
	assert(rb_apply(NULL, cb_add_one, NULL) == false);
	assert(rb_apply(&rb, cb_false, NULL) == false);

	rb_destroy(&rb);
}

NO_COV void test_cm_rb_naplly(void) {
	t_cm_rb rb     = {0};
	int     arr[3] = {1, 2, 3};
	int     out    = 0;

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 3);

	assert(rb_napply(&rb, 2, cb_add_one, NULL) == true);
	assert(*(int *)rb_pop(&rb, &out) == 2);
	assert(*(int *)rb_pop(&rb, &out) == 3);
	assert(*(int *)rb_pop(&rb, &out) == 3);
	rb_npush(&rb, arr, 3);
	assert(rb_napply(&rb, 2, cb_false, NULL) == false);
	assert(rb_napply(&rb, 1, NULL, NULL) == false);
	assert(rb_napply(NULL, 1, cb_add_one, NULL) == false);

	rb_destroy(&rb);
}
