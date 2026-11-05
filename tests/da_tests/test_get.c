#include <assert.h>

#include "da_tests.h"

NO_COV void test_cm_da_at(void) {
	const t_cm_da da = {
	    .array = (void *)&arr_3,
	    .cap_e = 3,
	    .nb_e  = 3,
	    .sz_e  = sizeof(int),
	};

	assert(*(int *)da_at(&da, 0) == arr_3[0]);
	assert(*(int *)da_at(&da, 1) == arr_3[1]);
	assert(*(int *)da_at(&da, 2) == arr_3[2]);
	assert(da_at(NULL, 0) == NULL);
	assert(da_at(&da, 99) == NULL);
}
NO_COV void test_cm_da_get(void) {
	const t_cm_da da = {
	    .array = (void *)&arr_3,
	    .cap_e = 3,
	    .nb_e  = 3,
	    .sz_e  = sizeof(int),
	};
	int out;

	assert(da_get(&da, 0, &out) == 1);
	assert(out == 1);
	assert(da_get(&da, 1, &out) == 1);
	assert(out == 2);
	assert(da_get(&da, 2, &out) == 1);
	assert(out == 3);

	out = 44444444;
	assert(da_get(&da, 99, &out) == -1);
	assert(out == 44444444);
	assert(da_get(&da, 99, NULL) == -1);
	assert(da_get(&da, 1, NULL) == -1);
	assert(da_get(NULL, 1, &out) == -1);
}
NO_COV void test_cm_da_nget(void) {
	const t_cm_da da = {
	    .array = (void *)&arr_3,
	    .cap_e = 3,
	    .nb_e  = 3,
	    .sz_e  = sizeof(int),
	};
	int buff[3];
	int out;

	assert(da_nget(&da, 1, 0, &out) == 1);
	assert(out == 1);
	assert(da_nget(&da, 2, 0, &buff) == 2);
	assert(buff[0] == 1);
	assert(buff[1] == 2);
	assert(da_nget(&da, -1, 0, &buff) == 3);
	assert(buff[0] == 1);
	assert(buff[1] == 2);
	assert(buff[2] == 3);
	assert(da_nget(&da, 0, 1, &buff) == 0);
	assert(da_nget(NULL, -1, 0, &buff) == -1);
	assert(da_nget(NULL, -1, 0, NULL) == -1);
	assert(da_nget(&da, -1, 0, NULL) == -1);
	assert(da_nget(&da, 1, 99, &buff) == -1);
}
NO_COV void test_cm_da_array(void) {
	const t_cm_da da = {
	    .array = (void *)&arr_3,
	    .cap_e = 3,
	    .nb_e  = 3,
	    .sz_e  = sizeof(int),
	};

	assert(da_array(&da) == da.array);
	assert(da_array(NULL) == NULL);
}
