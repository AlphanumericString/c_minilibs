#include <assert.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "rb_tests.h"

NO_COV void test_cm_rb_is_magic(void) {
	t_cm_rb rb      = {0};
	int     heap[2] = {1, 2};

	assert(rb_is_magic((t_cm_rb){0}) == false);

	assert(rb_create(&rb, sizeof(int), 4) != NULL);
	assert(rb_is_magic(rb) == true);
	rb_destroy(&rb);
	assert(rb_is_magic(rb) == false);

	// soft fallback is not magic
	shm_open_set_call_limit(0);
	assert(rb_create(&rb, sizeof(int), 4) != NULL);
	assert(rb_is_magic(rb) == false);
	rb_destroy(&rb);
	shm_open_set_call_limit(SIZE_MAX);

	// from_array buffers are never magic
	assert(rb_from_array(&rb, heap, 2, sizeof(int)) != NULL);
	assert(rb_is_magic(rb) == false);
}

NO_COV void test_cm_rb_magic_alias(void) {
	t_cm_rb rb = {0};
	int    *a;

	assert(rb_create(&rb, sizeof(int), 4) != NULL);
	assert(rb_is_magic(rb) == true);
	a = rb.array;

	// both windows are the same physical pages
	for (size_t i = 0; i < rb.nb_max; i++)
		a[i] = (int)i + 1;
	for (size_t i = 0; i < rb.nb_max; i++)
		assert(a[i + rb.nb_max] == a[i]);

	// writes through the mirror land in the first window
	a[rb.nb_max + 3] = 99;
	assert(a[3] == 99);

	// element addresses are contiguous across the window edge
	rb.o_pos         = rb.nb_max - 1;
	rb.count         = 2;
	a[rb.nb_max - 1] = 7;
	a[0]             = 1;
	assert(*(int *)rb_at(rb, 0) == 7);
	assert(*(int *)rb_at(rb, 1) == 1);
	assert((unsigned char *)rb_at(rb, 1) ==
	       (unsigned char *)rb_at(rb, 0) + sizeof(int));

	rb_destroy(&rb);
}

NO_COV void test_cm_rb_magic_copy(void) {
	t_cm_rb rb = {0};
	size_t  cap;
	int    *arr;
	int    *out;
	void   *p;

	assert(rb_create(&rb, sizeof(int), 10) != NULL);
	cap = rb_capacity(rb);
	arr = malloc(sizeof(int) * cap);
	out = malloc(sizeof(int) * (cap + 2));
	assert(arr != NULL && out != NULL);
	for (size_t i = 0; i < cap; i++)
		arr[i] = (int)i + 1;
	rb_npush(&rb, arr, cap);

	rb.o_pos = cap - 2;

	// nat / npeek across the window edge
	memset(out, 0, sizeof(int) * (cap + 2));
	assert(rb_nat(rb, 0, 3, out) != NULL);
	assert(out[0] == (int)cap - 1 && out[1] == (int)cap && out[2] == 1);
	memset(out, 0, sizeof(int) * (cap + 2));
	assert(rb_npeek(rb, 3, out) != NULL);
	assert(out[0] == (int)cap - 1 && out[1] == (int)cap && out[2] == 1);

	// npop across the window edge
	memset(out, 0, sizeof(int) * (cap + 2));
	p = rb_npop(&rb, 3, out);
	assert(p != NULL);
	assert(out[0] == (int)cap - 1 && out[1] == (int)cap && out[2] == 1);
	assert(rb_size(rb) == cap - 3);
	assert(rb.o_pos == 1);

	// last element with a large n: only what is left is read, never past the
	// second window
	rb.o_pos = cap - 1;
	rb.count = cap;
	memset(out, 0, sizeof(int) * (cap + 2));
	assert(rb_nat(rb, cap - 1, cap, out) != NULL);
	assert(out[0] == (int)cap - 1 && out[1] == 0);

	free(arr);
	free(out);
	rb_destroy(&rb);
}

NO_COV void test_cm_rb_magic_bulk(void) {
	t_cm_rb rb = {0};
	size_t  cap;
	int    *arr;
	int    *out;

	assert(rb_create(&rb, sizeof(int), 10) != NULL);
	cap = rb_capacity(rb);
	arr = malloc(sizeof(int) * (cap + 4));
	out = malloc(sizeof(int) * (cap + 2));
	assert(arr != NULL && out != NULL);
	for (size_t i = 0; i < cap + 4; i++)
		arr[i] = (int)i + 1;

	// fill up to the window edge, then wrap with a bulk push
	rb_npush(&rb, arr, cap - 2);
	assert(rb.i_pos == cap - 2);
	rb_npush(&rb, arr + (cap - 2), 4);
	assert(rb.i_pos == 2);
	assert(rb_size(rb) == cap);
	assert(rb.o_pos == 2);
	assert(*(int *)rb_at(rb, 0) == 3);
	assert(*(int *)rb_at(rb, 1) == 4);
	assert(*(int *)rb_at(rb, cap - 3) == (int)cap);
	assert(*(int *)rb_at(rb, cap - 2) == (int)cap + 1);
	assert(*(int *)rb_at(rb, cap - 1) == (int)cap + 2);

	// a bulk pop of the whole window starting at the wrapped head
	memset(out, 0, sizeof(int) * (cap + 2));
	rb.o_pos = cap - 2;
	assert(rb_npop(&rb, cap, out) != NULL);
	assert(out[0] == (int)cap - 1);
	assert(out[1] == (int)cap);
	assert(out[2] == (int)cap + 1);
	assert(out[cap - 1] == (int)cap - 2);
	assert(rb_size(rb) == 0);

	// push more than a window, the last window is kept
	rb_npush(&rb, arr, cap + 4);
	assert(rb_size(rb) == cap);
	assert(*(int *)rb_at(rb, 0) == 5);
	assert(*(int *)rb_at(rb, cap - 1) == (int)cap + 4);

	free(arr);
	free(out);
	rb_destroy(&rb);
}

NO_COV void test_cm_rb_from_array_paths(void) {
	t_cm_rb rb = {0};
	int    *heap;
	int     vals[] = {1, 2};
	int     out[4] = {0};

	heap = malloc(sizeof(int) * 4);
	assert(heap != NULL);
	heap[0] = 10;
	heap[1] = 20;
	heap[2] = 30;
	heap[3] = 40;
	assert(rb_from_array(&rb, heap, 4, sizeof(int)) != NULL);
	assert(rb_is_magic(rb) == false);

	// non mirrored bulk push crossing the window edge
	rb.count = 1;
	rb.o_pos = 0;
	rb.i_pos = 3;
	rb_npush(&rb, vals, 2);
	assert(rb_size(rb) == 3);
	assert(rb.i_pos == 1);
	assert(heap[3] == 1 && heap[0] == 2);

	// non mirrored bulk pop, single chunk then wrapped chunk
	memset(out, 0, sizeof(out));
	assert(rb_npop(&rb, 2, out) != NULL);
	assert(out[0] == 2 && out[1] == 20);
	assert(rb.o_pos == 2 && rb_size(rb) == 1);
	memset(out, 0, sizeof(out));
	assert(rb_npeek(rb, 1, out) != NULL);
	assert(out[0] == 30);

	// wrapped peek through the two chunks path
	heap[0]  = 3;
	rb.count = 2;
	rb.o_pos = 3;
	memset(out, 0, sizeof(out));
	assert(rb_npeek(rb, 2, out) != NULL);
	assert(out[0] == 1 && out[1] == 3);

	// non mirrored single chunk bulk push
	rb.count = 0;
	rb.o_pos = 0;
	rb.i_pos = 0;
	rb_npush(&rb, vals, 2);
	assert(rb_size(rb) == 2 && rb.i_pos == 2);

	rb_destroy(&rb); // frees heap
}
