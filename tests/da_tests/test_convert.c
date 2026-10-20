#include <assert.h>
#include <stdlib.h>

#include "da_tests.h"

NO_COV void test_da_to_array(void) {
	const t_da da = {
	    .array = (void *)&arr_5,
	    .cap_e = 5,
	    .nb_e  = 5,
	    .sz_e  = sizeof(int),
	};
	const t_da da_err = {
	    .array = NULL,
	    .cap_e = 99,
	    .nb_e  = 56,
	    .sz_e  = 3,
	};
	int *arr;

	alloc_set_call_limit(0);
	assert((int *)da_to_array(&da) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	arr = (int *)da_to_array(&da);
	assert(arr != NULL);
	assert(arr[0] == 1);
	assert(arr[1] == 2);
	assert(arr[2] == 3);
	assert(arr[3] == 4);
	assert(arr[4] == 5);
	free(arr);

	assert(da_to_array(NULL) == NULL);
	assert(da_to_array(&da_err) == NULL);
}
NO_COV void test_da_to_loc_array(void) {
	const t_da da = {
	    .array = (void *)&arr_5,
	    .cap_e = 5,
	    .nb_e  = 1,
	    .sz_e  = sizeof(int),
	};
	const t_da da2 = {
	    .array = (void *)&arr_5,
	    .cap_e = 5,
	    .nb_e  = 5,
	    .sz_e  = sizeof(int),
	};
	int arr[5] = {42, 42, 42, 42, 42};

	assert(da_to_loc_array(&da, arr) == arr);
	assert(arr[0] == 1);
	assert(arr[1] == 42);
	assert(da_to_loc_array(&da2, arr) == arr);
	assert(arr[0] == 1);
	assert(arr[1] == 2);
	assert(arr[2] == 3);
	assert(arr[3] == 4);
	assert(arr[4] == 5);
	assert(da_to_loc_array(&da2, NULL) == NULL);
	assert(da_to_loc_array(NULL, arr) == NULL);
}
