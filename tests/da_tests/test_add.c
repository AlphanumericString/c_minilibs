#include <assert.h>

#include "da_tests.h"

void test_da_add(void) {
	t_da         da;
	int          a;
	const size_t first_growth = C_MINI_DA__NB_ELEM_DFLT * C_MINI_DA__GROWTH;

	da_init(&da, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	a = 1;
	da_add(&da, &a);
	assert(((int *)(da.array))[da.nb_e - 1] == 1);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 1);
	a = 2;
	da_add(&da, &a);
	assert(((int *)(da.array))[da.nb_e - 1] == 2);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 2);
	a = 3;
	da_add(&da, &a);
	assert(((int *)(da.array))[da.nb_e - 1] == 3);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 3);
	a = 4;
	da_add(&da, &a);
	assert(((int *)(da.array))[da.nb_e - 1] == 4);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 4);
	a = 5;
	da_add(&da, &a);
	assert(((int *)(da.array))[da.nb_e - 1] == 5);
	ASSERT_DA_STATE(da, sizeof(int), first_growth, 5);
	da_destroy(&da);

	da_add(NULL, &a);
	da = (t_da){.array = (void *)0xDEAD, .cap_e = 0, .nb_e = 99, .sz_e = 1};
	da_add(&da, NULL);
	da = (t_da){.array = NULL, .cap_e = 99, .nb_e = 99, .sz_e = 1};
	da_add(&da, (void *)0xDEAD);
	da = (t_da){.array = (void *)0xDEAD, .cap_e = 0, .nb_e = 99, .sz_e = 1};
	da_add(&da, (void *)0xDEAD);
}
void test_da_cat_array(void) {
	t_da da;
	int  arr[]  = {1, 2, 3};
	int  arr2[] = {4, 5, 6};

	da_init(&da, sizeof(int));
	da_cat_array(&da, arr, 3, sizeof(int));
	assert(da.nb_e == 3);
	assert(((int *)da.array)[0] == arr[0]);
	assert(((int *)da.array)[1] == arr[1]);
	assert(((int *)da.array)[2] == arr[2]);
	da_cat_array(&da, arr2, 3, sizeof(int));
	assert(da.nb_e == 6);
	assert(((int *)da.array)[0] == arr[0]);
	assert(((int *)da.array)[1] == arr[1]);
	assert(((int *)da.array)[2] == arr[2]);
	assert(((int *)da.array)[3] == arr2[0]);
	assert(((int *)da.array)[4] == arr2[1]);
	assert(((int *)da.array)[5] == arr2[2]);
	da_destroy(&da);

	da = (t_da){.array = (void *)0xDEAD, .cap_e = 99, .nb_e = 99, .sz_e = 1};
	da_cat_array(&da, NULL, 12, 1);
	da_cat_array(NULL, (void *)0xDEAD, 12, 1);
	da_cat_array(&da, (void *)0xDEAD, 12, 0);
	da = (t_da){.array = NULL, .cap_e = 99, .nb_e = 99, .sz_e = 1};
	da_cat_array(&da, (void *)0xDEAD, 12, 1);
	da = (t_da){.array = (void *)0xDEAD, .cap_e = 99, .nb_e = 99, .sz_e = 1};
	da_cat_array(&da, (void *)0xDEAD, 12, 12);
}
