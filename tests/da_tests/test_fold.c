#include <assert.h>
#include <stdlib.h>

#include "da_tests.h"

void test_da_fold(void) {
	t_da da;
	int  sum = 0;
	int  a   = 1;

	da_init(&da, sizeof(int));
	da_add(&da, &a);
	da_fold(&da, cb_merger, &sum);
	assert(sum == 1);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	sum = 0;
	da_fold(&da, cb_merger, &sum);
	assert(sum == (1 + 2 + 3 + 4 + 5));
	da_destroy(&da);
}
void test_da_nfold(void) {
	t_da da;
	int  sum = 0;
	int  a   = 1;

	da_init(&da, sizeof(int));
	da_add(&da, &a);
	da_nfold(&da, 1, cb_merger, &sum);
	assert(sum == 1);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	sum = 0;
	da_nfold(&da, 1, cb_merger, &sum);
	assert(sum == 1);
	sum = 0;
	da_nfold(&da, 3, cb_merger, &sum);
	assert(sum == (1 + 2 + 3));
	sum = 42;
	da_nfold(&da, -1, cb_merger, &sum);
	assert(sum == (42 + (1 + 2 + 3 + 4 + 5)));
	da_destroy(&da);
}
void test_da_reduce(void) {
	t_da da;
	int  a = 1;
	int *res;

	da_init(&da, sizeof(int));
	da_add(&da, &a);
	res = da_reduce(&da, cb_merger);
	assert(res != NULL);
	assert(*res == 1);
	free(res);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	res = da_reduce(&da, cb_merger);
	assert(res != NULL);
	assert(*res == (1 + 2 + 3 + 4 + 5));
	free(res);
	da_destroy(&da);
}
void test_da_nreduce(void) {
	t_da da;
	int  a = 1;
	int *res;

	da_init(&da, sizeof(int));
	da_add(&da, &a);
	res = da_nreduce(&da, 1, cb_merger);
	assert(res != NULL);
	assert(*res == 1);
	free(res);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	res = da_nreduce(&da, 3, cb_merger);
	assert(res != NULL);
	assert(*res == (1 + 2 + 3));
	free(res);
	res = da_nreduce(&da, -1, cb_merger);
	assert(res != NULL);
	assert(*res == (1 + 2 + 3 + 4 + 5));
	free(res);
	da_destroy(&da);
}
