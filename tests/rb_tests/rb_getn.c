#include <assert.h>
#include <string.h>

#include "rb_tests.h"

NO_COV void test_cm_rb_getn(void) {
	t_cm_rb rb     = {0};
	int     arr[4] = {1, 2, 1, 3};
	int     out[4] = {0};

	const int target            = 1;
	const int target_only_once  = 2;
	const int target_not_in_set = 99;

	rb_create(&rb, sizeof(int), 4);
	rb_npush(&rb, arr, 4);

	// two matches available, ask for two
	memset(out, 0, sizeof(out));
	assert(rb_getn(&rb, 2, cb_cmp, &target, out) != NULL);
	assert(out[0] == 1 && out[1] == 1 && out[2] == 0);

	// ask for more than available: only the matches are copied
	memset(out, 0, sizeof(out));
	assert(rb_getn(&rb, 4, cb_cmp, &target, out) != NULL);
	assert(out[0] == 1 && out[1] == 1 && out[2] == 0 && out[3] == 0);

	// single match
	memset(out, 0, sizeof(out));
	assert(rb_getn(&rb, 4, cb_cmp, &target_only_once, out) != NULL);
	assert(out[0] == 2 && out[1] == 0);

	// no match
	assert(rb_getn(&rb, 4, cb_cmp, &target_not_in_set, out) == NULL);

	// wrap-around: move o_pos and push back on the input head
	rb_drop(&rb, 1);
	rb_push(&rb, &target); // order from o_pos: {2, 1, 3, 1}
	memset(out, 0, sizeof(out));
	assert(rb_getn(&rb, 2, cb_cmp, &target, out) != NULL);
	assert(out[0] == 1 && out[1] == 1);

	// bad arguments
	assert(rb_getn(&rb, 0, cb_cmp, &target, out) == NULL);
	assert(rb_getn(&rb, 2, cb_cmp, &target, NULL) == NULL);
	assert(rb_getn(&rb, 2, NULL, &target, out) == NULL);
	assert(rb_getn(NULL, 2, cb_cmp, &target, out) == NULL);

	rb_destroy(&rb);
}
