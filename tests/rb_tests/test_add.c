#include <assert.h>
#include <stdlib.h>

#include "rb_tests.h"

NO_COV void test_cm_rb_push(void) {
	t_cm_rb rb = {0};
	size_t  cap;
	int     val1 = 42;
	int     val2 = 43;
	int     val3 = 44;
	int     val;

	assert(rb_create(&rb, sizeof(int), 10) != NULL);
	cap = rb_capacity(rb);

	rb_push(&rb, &val1);
	assert(*(int *)rb_at(rb, 0) == val1);
	assert(rb_size(rb) == 1);
	assert(rb.o_pos == 0 && rb.i_pos == 1);

	// fill the whole magic window
	for (size_t i = 0; i < cap - 1; i++) {
		val = (int)i;
		rb_push(&rb, &val);
	}
	assert(rb_size(rb) == cap && rb.o_pos == 0 && rb.i_pos == 0);

	// one more push overwrites the oldest element
	rb_push(&rb, &val1);
	assert(rb_size(rb) == cap);
	assert(rb.o_pos == 1 && rb.i_pos == 1);
	assert(*(int *)rb_at(rb, 0) == 0);
	assert(*(int *)rb_at(rb, cap - 1) == val1);

	rb_push(&rb, &val2);
	rb_push(&rb, &val3);
	assert(*(int *)rb_at(rb, cap - 1) == val3);

	rb_push(&rb, NULL);
	rb_push(NULL, &val1);

	rb_destroy(&rb);
}

NO_COV void test_cm_rb_npush(void) {
	t_cm_rb rb = {0};
	size_t  cap;
	int    *big;

	assert(rb_create(&rb, sizeof(int), 10) != NULL);
	cap = rb_capacity(rb);
	big = malloc(sizeof(int) * (cap + 4));
	assert(big != NULL);
	for (size_t i = 0; i < cap + 4; i++)
		big[i] = (int)i + 1;

	// more than capacity: only the last cap elements are kept
	rb_npush(&rb, big, cap + 4);
	assert(rb_size(rb) == cap);
	assert(rb.o_pos == 0 && rb.i_pos == 0);
	assert(*(int *)rb_at(rb, 0) == 5);
	assert(*(int *)rb_at(rb, cap - 1) == (int)cap + 4);

	// exactly a window, as a single contiguous write
	rb_clr(&rb);
	rb_npush(&rb, big, cap);
	assert(rb_size(rb) == cap);
	assert(*(int *)rb_at(rb, 0) == 1);
	assert(*(int *)rb_at(rb, cap - 1) == (int)cap);
	assert(rb.o_pos == 0 && rb.i_pos == 0);

	// partial write keeps prior content in place
	rb_clr(&rb);
	rb_npush(&rb, big, 3);
	assert(rb_size(rb) == 3 && rb.i_pos == 3);
	rb_npush(&rb, big + 3, 2);
	assert(rb_size(rb) == 5 && rb.i_pos == 5);
	assert(*(int *)rb_at(rb, 0) == 1);
	assert(*(int *)rb_at(rb, 4) == 5);

	// overwrite by a few elements advances the output head only
	rb_clr(&rb);
	rb_npush(&rb, big, cap);
	rb_npush(&rb, big, 3);
	assert(rb_size(rb) == cap && rb.o_pos == 3 && rb.i_pos == 3);
	assert(*(int *)rb_at(rb, 0) == 4);

	rb_npush(&rb, NULL, 12);
	rb_npush(NULL, big, 12);
	free(big);
	rb_destroy(&rb);
}

NO_COV void test_cm_rb_push_safe(void) {
	t_cm_rb rb  = {0};
	int     val = 42;
	size_t  cap;

	assert(rb_create(&rb, sizeof(int), 10) != NULL);
	cap = rb_capacity(rb);

	for (size_t i = 0; i < cap; i++)
		assert(rb_push_safe(&rb, &val) == true);
	assert(rb_size(rb) == cap);
	assert(rb_push_safe(&rb, &val) == false);
	assert(rb_push_safe(NULL, &val) == false);
	assert(rb_push_safe(&rb, NULL) == false);

	rb_destroy(&rb);
}

NO_COV void test_cm_rb_npush_safe(void) {
	t_cm_rb rb = {0};
	size_t  cap;
	int    *arr;

	assert(rb_create(&rb, sizeof(int), 10) != NULL);
	cap = rb_capacity(rb);
	arr = malloc(sizeof(int) * (cap + 1));
	assert(arr != NULL);
	for (size_t i = 0; i < cap + 1; i++)
		arr[i] = (int)i;

	assert(rb_npush_safe(&rb, arr, cap + 1) == false); // n > capacity
	assert(rb_npush_safe(&rb, arr, cap) == true);
	assert(rb_size(rb) == cap && rb_capacity(rb) == cap);
	assert(rb_npush_safe(&rb, arr, 1) == false);
	assert(rb_npush_safe(&rb, arr, 0) == true);

	rb_clr(&rb);
	assert(rb_npush_safe(&rb, arr, cap - 1) == true);
	assert(rb_size(rb) == cap - 1);
	assert(rb_npush_safe(&rb, arr, 1) == true);
	assert(rb_size(rb) == cap);
	assert(rb_npush_safe(&rb, arr, 1) == false);
	assert(rb_npush_safe(&rb, NULL, 1) == false);
	assert(rb_npush_safe(NULL, arr, 1) == false);

	free(arr);
	rb_destroy(&rb);
}
