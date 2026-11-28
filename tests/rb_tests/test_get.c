#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "rb_tests.h"

NO_COV void test_cm_rb_get(void) {
	t_cm_rb rb     = {0};
	int     arr[4] = {1, 2, 3, 4};
	int     out    = 0;

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

NO_COV void test_cm_rb_nget(void) {
	t_cm_rb rb = {0};
	size_t  cap;
	int    *arr;
	int    *out;
	int     target;
	int     target_not_in_set;

	rb_create(&rb, sizeof(int), 10);
	cap               = rb_capacity(rb);
	target_not_in_set = (int)cap + 99;
	arr               = malloc(sizeof(int) * cap);
	out               = malloc(sizeof(int) * (cap + 2));
	assert(arr != NULL && out != NULL);
	for (size_t i = 0; i < cap; i++)
		arr[i] = (int)i + 1;
	rb_npush(&rb, arr, cap);

	target = 2;
	assert(rb_nget(&rb, 2, cb_cmp, &target, out) != NULL);
	assert(out[0] == 2 && out[1] == 3);

	// near the end of the live window: only the elements left are copied
	target = (int)cap - 1;
	memset(out, 0, sizeof(int) * (cap + 2));
	assert(rb_nget(&rb, 2, cb_cmp, &target, out) != NULL);
	assert(out[0] == (int)cap - 1 && out[1] == (int)cap);
	target = (int)cap;
	memset(out, 0, sizeof(int) * (cap + 2));
	assert(rb_nget(&rb, 2, cb_cmp, &target, out) != NULL);
	assert(out[0] == (int)cap && out[1] == 0);

	assert(rb_nget(&rb, 2, cb_cmp, &target_not_in_set, out) == NULL);
	// assert(out[0] == 0 && out[1] == 0);

	assert(rb_nget(&rb, 0, cb_cmp, &target, out) == NULL);
	assert(rb_nget(&rb, 2, cb_cmp, &target, NULL) == NULL);
	assert(rb_nget(&rb, 2, NULL, &target, out) == NULL);
	assert(rb_nget(NULL, 2, cb_cmp, &target, out) == NULL);

	free(arr);
	free(out);
	rb_destroy(&rb);
}
