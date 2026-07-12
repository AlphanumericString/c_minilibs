#include "da_tests.h"

// get
void test_da_at(void) {
	t_da da;
	int  a;

	da_init(&da, sizeof(int));
	a = 1;
	da_add(&da, &a);
	assert(*(int *)da_at(&da, 0) == 1);
	da_destroy(&da);
}
void test_da_get(void) {
	t_da da;
	int  a, out;

	da_init(&da, sizeof(int));
	a = 1;
	da_add(&da, &a);

	da_get(&da, 0, &out);
	assert(out == 1);

	da_destroy(&da);
}
void test_da_nget(void) {
	t_da da;
	int  buff[2];
	int  out;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));

	da_nget(&da, 1, 0, &out);
	assert(out == 1);
	da_nget(&da, 2, 0, &buff);
	assert(buff[0] == 1);
	assert(buff[1] == 2);

	da_destroy(&da);
}
void test_da_array(void) {
	t_da da;

	da_init(&da, sizeof(int));

	assert(da_array(&da) == da.array);
	da_cat_array(&da, (int[]){1, 43, 4, 5, 223}, 5, sizeof(int));
	assert(da_array(&da) == da.array);

	da_destroy(&da);
}
