#include <assert.h>

#include "rb_tests.h"

void test_rb_get(void) {
	t_rb rb     = {0};
	int  arr[4] = {1, 2, 3, 4};
	int  out    = 0;

	const int target            = 3;
	const int target_not_in_set = 99;

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 4);

	assert(rb_get(&rb, cb_cmp, &target, &out) != NULL);
	assert(out == 3);
	assert(rb_get(&rb, cb_cmp, &target_not_in_set, &out) == NULL);
	// assert(out == 0);
	assert(rb_get(&rb, cb_cmp, &target, NULL) == NULL);
	assert(rb_get(&rb, NULL, &target, &out) == NULL);
	assert(rb_get(NULL, cb_cmp, &target, &out) == NULL);

	rb_destroy(&rb);
}

void test_rb_nget(void) {
	t_rb rb     = {0};
	int  arr[4] = {1, 2, 3, 4};
	int  out[2] = {0};

	int       target            = 2;
	const int target_not_in_set = 99;

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 4);

	assert(rb_nget(&rb, 2, cb_cmp, &target, out) != NULL);
	assert(out[0] == 2 && out[1] == 3);
	target = 4;
	assert(rb_nget(&rb, 2, cb_cmp, &target, out) != NULL);
	assert(out[0] == 4 && out[1] == 1);
	assert(rb_nget(&rb, 2, cb_cmp, &target_not_in_set, out) == NULL);
	// assert(out[0] == 0 && out[1] == 0);

	assert(rb_nget(&rb, 0, cb_cmp, &target, out) == NULL);
	assert(rb_nget(&rb, 2, cb_cmp, &target, NULL) == NULL);
	assert(rb_nget(&rb, 2, NULL, &target, &out) == NULL);
	assert(rb_nget(NULL, 2, cb_cmp, &target, &out) == NULL);

	rb_destroy(&rb);
}
