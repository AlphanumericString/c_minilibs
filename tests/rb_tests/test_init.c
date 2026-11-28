#include <assert.h>
#include <stdint.h>
#include <stdlib.h>

#include "rb_tests.h"

NO_COV void test_cm_rb_create(void) {
	t_cm_rb rb     = {0};
	size_t  page_c = RB_TEST_PAGE_CAP(sizeof(int), 10);

	assert(rb_create(&rb, sizeof(int), 10) != NULL);
	assert(rb_is_magic(rb) == true);
	assert(rb.nb_max == page_c);
	assert(rb.map_size == RB_TEST_ROUND_BYTES(sizeof(int) * 10));
	ASSERT_RB_STATE(rb, sizeof(int), page_c, 0, 0)
	rb_destroy(&rb);

	assert(rb_create(&rb, 0, 10) == NULL);
	assert(rb_create(&rb, sizeof(int), 0) == NULL);
	assert(rb_create(NULL, sizeof(int), 10) == NULL);
	assert(rb_create(&rb, sizeof(int), SIZE_MAX) == NULL);

	// mapping failure falls back on malloc
	shm_open_set_call_limit(0);
	assert(rb_create(&rb, sizeof(int), 10) != NULL);
	assert(rb_is_magic(rb) == false);
	assert(rb.nb_max == 10 && rb.map_size == 0);
	rb_destroy(&rb);
	shm_open_set_call_limit(SIZE_MAX);

	// mapping failure and malloc failure
	shm_open_set_call_limit(0);
	alloc_set_call_limit(0);
	assert(rb_create(&rb, sizeof(int), 10) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	shm_open_set_call_limit(SIZE_MAX);

	// page size unavailable: the mirror is impossible, soft fallback
	sysconf_set_call_limit(0);
	assert(rb_create(&rb, sizeof(int), 10) != NULL);
	assert(rb_is_magic(rb) == false);
	rb_destroy(&rb);
	sysconf_set_call_limit(SIZE_MAX);
}

NO_COV void test_cm_rb_create_magic(void) {
	t_cm_rb rb     = {0};
	size_t  page_c = RB_TEST_PAGE_CAP(sizeof(int), 10);

	assert(rb_create_magic(&rb, sizeof(int), 10) != NULL);
	assert(rb_is_magic(rb) == true);
	assert(rb.nb_max == page_c);
	rb_destroy(&rb);

	assert(rb_create_magic(&rb, 0, 10) == NULL);
	assert(rb_create_magic(&rb, sizeof(int), 0) == NULL);
	assert(rb_create_magic(NULL, sizeof(int), 10) == NULL);

	// byte sizes that overflow the mapping arithmetic are rejected
	assert(rb_create_magic(&rb, 1, SIZE_MAX) == NULL);
	assert(rb_create_magic(&rb, 1, SIZE_MAX / 2 + 4096) == NULL);

	// every failure point of the mapping chain is a hard failure here
	sysconf_set_call_limit(0); // page size unavailable
	assert(rb_create_magic(&rb, sizeof(int), 10) == NULL);
	sysconf_set_call_limit(SIZE_MAX);

	shm_open_set_call_limit(0);
	assert(rb_create_magic(&rb, sizeof(int), 10) == NULL);
	shm_open_set_call_limit(SIZE_MAX);

	ftruncate_set_call_limit(0);
	assert(rb_create_magic(&rb, sizeof(int), 10) == NULL);
	ftruncate_set_call_limit(SIZE_MAX);

	mmap_set_call_limit(0); // address space reservation
	assert(rb_create_magic(&rb, sizeof(int), 10) == NULL);
	mmap_set_call_limit(SIZE_MAX);

	mmap_set_call_limit(1); // first window
	assert(rb_create_magic(&rb, sizeof(int), 10) == NULL);
	mmap_set_call_limit(SIZE_MAX);

	mmap_set_call_limit(2); // mirrored window
	assert(rb_create_magic(&rb, sizeof(int), 10) == NULL);
	mmap_set_call_limit(SIZE_MAX);
}

NO_COV void test_cm_rb_init(void) {
	t_cm_rb rb = {0};

	assert(rb_init(&rb, sizeof(int)) != NULL);
	assert(rb_is_magic(rb) == true);
	ASSERT_RB_STATE(rb, sizeof(int),
	                RB_TEST_PAGE_CAP(sizeof(int), C_MINI__RB_NBE_DFLT), 0, 0)

	rb_destroy(&rb);
	assert(rb_init(NULL, sizeof(int)) == NULL);
	assert(rb_init(&rb, 0) == NULL);
	shm_open_set_call_limit(0);
	assert(rb_init(&rb, sizeof(int)) != NULL); // soft fallback
	assert(rb_is_magic(rb) == false);
	rb_destroy(&rb);
	alloc_set_call_limit(0);
	assert(rb_init(&rb, sizeof(int)) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	shm_open_set_call_limit(SIZE_MAX);

	assert(rb_init_magic(&rb, sizeof(int)) != NULL);
	assert(rb_is_magic(rb) == true);
	rb_destroy(&rb);
	assert(rb_init_magic(NULL, sizeof(int)) == NULL);
	shm_open_set_call_limit(0);
	assert(rb_init_magic(&rb, sizeof(int)) == NULL);
	shm_open_set_call_limit(SIZE_MAX);
}

NO_COV void test_cm_rb_from_array(void) {
	t_cm_rb rb     = {0};
	int     arr[5] = {1, 2, 3, 4, 5};

	assert(rb_from_array(&rb, arr, 5, sizeof(int)) != NULL);
	assert(rb_is_magic(rb) == false);
	assert(rb.map_size == 0);
	ASSERT_RB_STATE(rb, sizeof(int), 5, 0, 0)
	assert(((int *)rb.array)[0] == 1);
	assert(((int *)rb.array)[4] == 5);

	assert(rb_from_array(NULL, arr, 5, sizeof(int)) == NULL);
	assert(rb_from_array(&rb, NULL, 5, sizeof(int)) == NULL);
	assert(rb_from_array(&rb, arr, 0, sizeof(int)) == NULL);
	assert(rb_from_array(&rb, arr, 5, 0) == NULL);
}

NO_COV void test_cm_rb_from_array_owned(void) {
	t_cm_rb rb = {0};
	int    *arr;

	// from_array takes ownership, so destroy must free the heap array
	arr = malloc(sizeof(int) * 3);
	assert(arr != NULL);
	arr[0] = 1;
	arr[1] = 2;
	arr[2] = 3;
	assert(rb_from_array(&rb, arr, 3, sizeof(int)) != NULL);
	rb_destroy(&rb);
	assert(rb.array == NULL && rb.map_size == 0 && rb.mirrored == false);
}

NO_COV void test_cm_rb_new(void) {
	t_cm_rb *rb = NULL;

	rb = rb_new(sizeof(int));
	assert(rb != NULL && rb_is_magic(*rb) == true);
	ASSERT_RB_STATE(*rb, sizeof(int),
	                RB_TEST_PAGE_CAP(sizeof(int), C_MINI__RB_NBE_DFLT), 0, 0)
	rb_free(&rb);
	assert(rb == NULL);

	assert(rb_new(0) == NULL);
	alloc_set_call_limit(0);
	assert(rb_new(32) == NULL);

	// only the struct is malloc'd, the array is mapped: one alloc is enough
	alloc_reset_counter();
	alloc_set_call_limit(1);
	rb = rb_new(32);
	assert(rb != NULL);
	rb_free(&rb);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	rb = rb_new_magic(sizeof(int));
	assert(rb != NULL && rb_is_magic(*rb) == true);
	rb_free(&rb);
	assert(rb_new_magic(0) == NULL);
	shm_open_set_call_limit(0);
	assert(rb_new_magic(sizeof(int)) == NULL);
	shm_open_set_call_limit(SIZE_MAX);
	alloc_reset_counter();

	// struct allocation fails, then struct succeeds but the backing fails:
	// no leak in either case
	alloc_set_call_limit(0);
	alloc_reset_counter();
	assert(rb_new_magic(sizeof(int)) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	shm_open_set_call_limit(0);
	alloc_reset_counter();
	alloc_set_call_limit(1);
	assert(rb_new(sizeof(int)) == NULL);
	alloc_reset_counter();
	assert(rb_new_magic(sizeof(int)) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	shm_open_set_call_limit(SIZE_MAX);
	alloc_reset_counter();
}
