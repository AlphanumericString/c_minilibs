#include <assert.h>

#include "da_tests.h"

NO_COV void test_da_set(void) {
	int  data[5] = {0};
	t_da da      = {.array = data, .cap_e = 5, .nb_e = 5, .sz_e = sizeof(int)};
	int  b       = 1;

	da_set(&da, 0, &b);
	b++;
	da_set(&da, 1, &b);
	b++;
	da_set(&da, 2, &b);
	b++;
	da_set(&da, 3, &b);
	b++;
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	da_set(&da, 99, &b);
	da_set(NULL, 0, &b);
	da_set(&da, 3, NULL);
}
NO_COV void test_da_nset(void) {
	t_da da;
	int  b       = 2;
	int  buff[2] = {3, 4};

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));
	assert(da_nset(&da, 1, 0, &b) == true);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(da_nset(&da, 2, 0, &buff) == true);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(da_nset(&da, 0, 0, &buff) == true);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);

	assert(da_nset(NULL, 1, 0, &b) == false);
	assert(da_nset(&da, 1, 0, NULL) == false);
	assert(da_nset(&da, 1, 999, &b) == false);
	assert(da_nset(&da, 999, 0, &b) == false);
	da_destroy(&da);
}
