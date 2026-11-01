#include <assert.h>

#include "rb_tests.h"

NO_COV void test_rb_push(void) {
	t_rb rb   = {0};
	int  val1 = 42;
	int  val2 = 43;
	int  val3 = 44;

	rb_create(&rb, sizeof(int), 2);

	rb_push(&rb, &val1);
	assert(*(int *)rb_at(rb, 0) == val1);
	assert(rb_size(rb) == 1);
	assert(rb.o_pos == 0);
	rb_push(&rb, &val2);
	assert(*(int *)rb_at(rb, 0) == val1);
	assert(*(int *)rb_at(rb, 1) == val2);
	assert(rb_size(rb) == 2);
	assert(rb.o_pos == 0);
	rb_push(&rb, &val3);
	assert(*(int *)rb_at(rb, 0) == val2);
	assert(*(int *)rb_at(rb, 1) == val3);
	assert(rb_size(rb) == 2);
	assert(rb.o_pos == 1);

	rb_push(&rb, NULL);
	rb_push(NULL, &val1);

	rb_destroy(&rb);
}

NO_COV void test_rb_npush(void) {
	t_rb rb          = {0};
	int  arr[3]      = {1, 2, 3};
	int  arr_large[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12};

	rb_create(&rb, sizeof(int), 4);

	rb_npush(&rb, arr, 3);
	CM_DBG("rb_at(rb, 0) == %d", *(int *)rb_at(rb, 0));
	CM_DBG("rb_at(rb, 1) == %d", *(int *)rb_at(rb, 1));
	CM_DBG("rb_at(rb, 2) == %d", *(int *)rb_at(rb, 2));
	CM_DBG("rb_at(rb, 3) == %p", (int *)rb_at(rb, 3));
	assert(rb_size(rb) == 3);
	assert(rb.o_pos == 0);
	assert(rb.i_pos == 3);
	// i_pos at 3 - cycle "moved" by 3
	rb_npush(&rb, arr_large, sizeof(arr_large) / sizeof(arr_large[0]));

	assert(rb.o_pos == 3);
	assert(*(int *)rb_at(rb, 0) == 9);
	assert(*(int *)rb_at(rb, 1) == 10);
	assert(*(int *)rb_at(rb, 2) == 11);
	assert(*(int *)rb_at(rb, 3) == 12);

	rb_clr(&rb);
	assert(rb.o_pos == 0);
	// i_pos at 0 - cycle "fixed"
	rb_npush(&rb, arr_large, sizeof(arr_large) / sizeof(arr_large[0]));
	assert(rb_size(rb) == 4);

	assert(*(int *)rb_at(rb, 0) == 9);
	assert(*(int *)rb_at(rb, 1) == 10);
	assert(*(int *)rb_at(rb, 2) == 11);
	assert(*(int *)rb_at(rb, 3) == 12);

	rb_npush(&rb, NULL, 12);
	rb_npush(NULL, arr, 12);
	rb_destroy(&rb);
}

NO_COV void test_rb_push_safe(void) {
	t_rb rb  = {0};
	int  val = 42;

	rb_create(&rb, sizeof(int), 2);

	assert(rb_push_safe(&rb, &val) == true);
	assert(rb_push_safe(&rb, &val) == true);
	assert(rb_push_safe(&rb, &val) == false);
	assert(rb_push_safe(NULL, &val) == false);
	assert(rb_push_safe(&rb, false) == false);

	rb_destroy(&rb);
}

NO_COV void test_rb_npush_safe(void) {
	t_rb rb     = {0};
	int  arr[3] = {1, 2, 3};

	rb_create(&rb, sizeof(int), 2);

	assert(rb_npush_safe(&rb, arr, 3) == false);
	assert(rb_size(rb) == 0);
	rb_destroy(&rb);
	rb_create(&rb, sizeof(int), 4);
	assert(rb_npush_safe(&rb, arr, 3) == true);
	assert(rb_size(rb) == 3 && rb_capacity(rb) == 4);
	assert(rb_npush_safe(&rb, arr, 2) == false);
	assert(rb_size(rb) == 3 && rb_capacity(rb) == 4);
	assert(rb_npush_safe(&rb, arr, 1) == true);
	assert(rb_size(rb) == 4 && rb_capacity(rb) == 4);
	assert(rb_npush_safe(&rb, NULL, 12) == false);
	assert(rb_npush_safe(NULL, arr, 12) == false);

	rb_destroy(&rb);
}
