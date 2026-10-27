#include <assert.h>

#include "da_tests.h"

NO_COV void test_da_swap(void) {
	int  data[3] = {1, 2, 3};
	char large_data_blocks[(C_MINI_DA__SZ_ELEM_DFLT + 1) * 3];

	t_da da = {.array = data, .nb_e = 3, .cap_e = 3, .sz_e = sizeof(int)};
	t_da da_large_blocks = {
	    .array = large_data_blocks,
	    .nb_e  = 3,
	    .cap_e = 3,
	    .sz_e  = C_MINI_DA__SZ_ELEM_DFLT + 1,
	};
	*(int *)da_at(&da_large_blocks, 0) = 42;
	*(int *)da_at(&da_large_blocks, 1) = 24;

	assert(da_swap(&da, 0, 1) == true);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 1);
	assert(da_swap(&da_large_blocks, 0, 1) == true);
	assert(*(int *)da_at(&da_large_blocks, 0) == 24);
	assert(*(int *)da_at(&da_large_blocks, 1) == 42);
	assert(da_swap(&da, 12, 1) == false);
	assert(da_swap(&da, 1, 12) == false);
}
NO_COV void test_da_nswap(void) {
	int  data[5] = {1, 2, 3, 4, 5};
	char large_data_blocks[(C_MINI_DA__SZ_ELEM_DFLT + 1) * 3];

	t_da da = {.array = data, .nb_e = 5, .cap_e = 5, .sz_e = sizeof(int)};
	t_da da_large_blocks = {
	    .array = large_data_blocks,
	    .nb_e  = 3,
	    .cap_e = 3,
	    .sz_e  = C_MINI_DA__SZ_ELEM_DFLT + 1,
	};

	*(int *)da_at(&da_large_blocks, 0) = 42;
	*(int *)da_at(&da_large_blocks, 1) = 24;
	*(int *)da_at(&da_large_blocks, 2) = 12;

	assert(da_nswap(&da, 1, 0, 1) == true); // 2 1 3 4 5
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 1);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(da_nswap(&da, 2, 0, 2) == true); // 3 4 2 1 5
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 2);
	assert(*(int *)da_at(&da, 3) == 1);
	assert(da_nswap(&da, 0, 0, 2) == true);    // 3 4 2 1 5
	assert(da_nswap(&da, 99, 0, 2) == false);  // 3 4 2 1 5
	assert(da_nswap(&da, 1, 99, 2) == false);  // 3 4 2 1 5
	assert(da_nswap(&da, 1, 0, 99) == false);  // 3 4 2 1 5
	assert(da_nswap(NULL, 1, 0, 99) == false); // 3 4 2 1 5
	assert(da_nswap(&da_large_blocks, 1, 1, 0) == true);
	assert(*(int *)da_at(&da_large_blocks, 0) == 24);
	assert(*(int *)da_at(&da_large_blocks, 1) == 42);
	assert(*(int *)da_at(&da_large_blocks, 2) == 12);
	assert(da_nswap(&da_large_blocks, 1, 1, 0) == true);
	assert(*(int *)da_at(&da_large_blocks, 0) == 42);
	assert(*(int *)da_at(&da_large_blocks, 1) == 24);
	assert(*(int *)da_at(&da_large_blocks, 2) == 12);
	assert(da_nswap(&da_large_blocks, 1, 0, 0) == true); // no-op
	assert(*(int *)da_at(&da_large_blocks, 0) == 42);
	assert(*(int *)da_at(&da_large_blocks, 1) == 24);
	assert(*(int *)da_at(&da_large_blocks, 2) == 12);
	assert(da_nswap(&da_large_blocks, 2, 0, 1) == false);
	assert(*(int *)da_at(&da_large_blocks, 0) == 42);
	assert(*(int *)da_at(&da_large_blocks, 1) == 24);
	assert(*(int *)da_at(&da_large_blocks, 2) == 12);
	assert(da_nswap(&da_large_blocks, 2, 1, 0) == false);
	assert(*(int *)da_at(&da_large_blocks, 0) == 42);
	assert(*(int *)da_at(&da_large_blocks, 1) == 24);
	assert(*(int *)da_at(&da_large_blocks, 2) == 12);
}
