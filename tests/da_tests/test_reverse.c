#include <assert.h>

#include "da_tests.h"

NO_COV void test_cm_da_reverse(void) {
	int     data[5] = {1, 2, 3, 4, 5};
	t_cm_da da = {.array = data, .nb_e = 5, .cap_e = 5, .sz_e = sizeof(int)};

	da_reverse(&da);
	assert(*(int *)da_at(&da, 0) == 5);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 2);
	assert(*(int *)da_at(&da, 4) == 1);
	da_reverse(NULL);
}
NO_COV void test_cm_da_nreverse(void) {
	int     data[5] = {1, 2, 3, 4, 5};
	t_cm_da da = {.array = data, .nb_e = 5, .cap_e = 5, .sz_e = sizeof(int)};

	da_nreverse(&da, 2);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 1);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	da_nreverse(&da, 5);
	assert(*(int *)da_at(&da, 0) == 5);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 1);
	assert(*(int *)da_at(&da, 4) == 2);
	da_nreverse(NULL, 2);
	da_nreverse(&da, 0);
}
NO_COV void test_cm_da_b_nreverse(void) {
	int     data[5] = {1, 2, 3, 4, 5};
	t_cm_da da = {.array = data, .nb_e = 5, .cap_e = 5, .sz_e = sizeof(int)};

	da_b_nreverse(&da, 2);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 5);
	assert(*(int *)da_at(&da, 4) == 4);
	da_b_nreverse(&da, 5);
	assert(*(int *)da_at(&da, 0) == 4);
	assert(*(int *)da_at(&da, 1) == 5);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 2);
	assert(*(int *)da_at(&da, 4) == 1);
	da_b_nreverse(NULL, 2);
	da_b_nreverse(&da, 0);
}
