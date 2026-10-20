#include <assert.h>

#include "rb_tests.h"

NO_COV void test_rb_find(void) {
	t_rb      rb                = {0};
	int       arr[4]            = {1, 2, 3, 4};
	const int target            = 3;
	const int target_not_in_set = 99;
	int      *pos;

	rb_init(&rb, sizeof(int));
	rb_npush(&rb, arr, 4);

	pos = rb_find(&rb, cb_cmp, &target);
	assert(pos != NULL);
	assert(*pos == 3);
	pos = rb_find(&rb, cb_cmp, &target_not_in_set);
	assert(pos == NULL);

	pos = rb_find(&rb, NULL, &target);
	assert(pos == NULL);
	pos = rb_find(NULL, cb_cmp, &target);
	assert(pos == NULL);

	rb_destroy(&rb);
}
