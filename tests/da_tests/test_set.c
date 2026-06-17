#include "da_tests.h"

// set
void test_da_set(void) {
	t_da da;
	int  a = 1;
	int  b = 2;

	da_init(&da, sizeof(int));
	da_add(&da, &a);
	da_set(&da, 0, &b);

	assert(*(int *)da_at(&da, 0) == 2);

	da_destroy(&da);
}
void test_da_nset(void) {
	t_da da;
	int  b       = 2;
	int  buff[2] = {3, 4};

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));

	da_nset(&da, 1, 0, &b);
	assert(*(int *)da_at(&da, 0) == 2);
	da_nset(&da, 2, 0, &buff);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);

	da_destroy(&da);
}
