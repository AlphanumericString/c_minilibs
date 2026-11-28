#define C_MINI__RB_INTERNAL
#include <assert.h>

#include "rb_tests.h"

/* the internal helpers are not reachable through the public API: exercise
 * their defensive branches directly */
NO_COV void test_cm_rb_mirror_helpers(void) {
	t_cm_rb rb     = {0};
	void   *array  = NULL;
	size_t  size   = 0;
	int     stored = 42;
	int     other  = 0;

	assert(rbi_map_ring(0, &array, &size) == false);
	assert(rbi_map_ring(4096, NULL, &size) == false);
	assert(rbi_map_ring(4096, &array, NULL) == false);
	assert(array == NULL && size == 0);
	rbi_unmap_ring(NULL, 4096);
	rbi_unmap_ring(&stored, 0);

	rb.array  = &stored;
	rb.sz_e   = sizeof(int);
	rb.nb_max = 4;
	rbi_copy_out(&rb, 0, 1, &other);
	assert(other == 42);
	rbi_copy_out(&rb, 0, 0, &other);
	rbi_copy_out(&rb, 0, 1, NULL);
	rbi_copy_out(NULL, 0, 1, &other);
	other = 7;
	rbi_copy_in(&rb, 0, &other, 1);
	assert(stored == 7);
	rbi_copy_in(&rb, 0, NULL, 1);
	rbi_copy_in(NULL, 0, &other, 1);

	assert(rbi_wrap(&rb, 3) == 3);
	assert(rbi_wrap(&rb, 4) == 0);
	assert(rbi_wrap(&rb, 5) == 1);
	assert(rbi_off_at(&rb, 3, 2) == 1);
	rb.mirrored = true;
	assert(rbi_off_at(&rb, 3, 2) == 5);
}
