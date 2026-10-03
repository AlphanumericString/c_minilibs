#include <assert.h>
#include <stdlib.h>

#include "da_tests.h"

void test_da_fold(void) {
	int  sum = 0;
	t_da da  = {
	     .array = (void *)arr_5,
	     .cap_e = 5,
	     .nb_e  = 1,
	     .sz_e  = sizeof(int),
    };

	assert(da_fold(&da, cb_merger, &sum) != NULL);
	assert(sum == 1);
	da.nb_e = 5;
	sum     = 0;
	assert(da_fold(&da, cb_merger, &sum) != NULL);
	assert(sum == (1 + 2 + 3 + 4 + 5));
	assert(da_fold(NULL, cb_merger, &sum) == NULL);
	assert(da_fold(&da, cb_merger, NULL) == NULL);
	assert(da_fold(&da, NULL, &sum) == NULL);
}
void test_da_nfold(void) {
	t_da da = {
	    .array = (void *)arr_5,
	    .cap_e = 5,
	    .nb_e  = 5,
	    .sz_e  = sizeof(int),
	};
	int sum = 0;

	assert(da_nfold(&da, 1, cb_merger, &sum) != NULL);
	assert(sum == 1);
	sum = 0;
	assert(da_nfold(&da, 1, cb_merger, &sum) != NULL);
	assert(sum == 1);
	sum = 0;
	assert(da_nfold(&da, 3, cb_merger, &sum) != NULL);
	assert(sum == (1 + 2 + 3));
	sum = 42;
	assert(da_nfold(&da, -1, cb_merger, &sum) != NULL);
	assert(sum == (42 + (1 + 2 + 3 + 4 + 5)));
	assert(da_nfold(NULL, -1, cb_merger, &sum) == NULL);
	assert(da_nfold(&da, -1, NULL, &sum) == NULL);
	assert(da_nfold(&da, -1, cb_merger, NULL) == NULL);
}
void test_da_reduce(void) {
	int *res;
	t_da da = {
	    .array = (void *)arr_5,
	    .cap_e = 5,
	    .nb_e  = 1,
	    .sz_e  = sizeof(int),
	};

	res = da_reduce(&da, cb_merger);
	assert(res != NULL);
	assert(*res == 1);
	free(res);
	da.nb_e = 5;
	res     = da_reduce(&da, cb_merger);
	assert(res != NULL);
	assert(*res == (1 + 2 + 3 + 4 + 5));
	free(res);
	assert(da_reduce(NULL, cb_merger) == NULL);
	assert(da_reduce(&da, NULL) == NULL);
}
void test_da_nreduce(void) {
	int *res;
	t_da da = {
	    .array = (void *)arr_5,
	    .cap_e = 5,
	    .nb_e  = 5,
	    .sz_e  = sizeof(int),
	};

	res = da_nreduce(&da, 1, cb_merger);
	assert(res != NULL);
	assert(*res == 1);
	free(res);
	da.nb_e = 5;
	res     = da_nreduce(&da, 3, cb_merger);
	assert(res != NULL);
	assert(*res == (1 + 2 + 3));
	free(res);
	res = da_nreduce(&da, -1, cb_merger);
	assert(res != NULL);
	assert(*res == (1 + 2 + 3 + 4 + 5));
	free(res);
	assert(da_nreduce(NULL, -1, cb_merger) == NULL);
	assert(da_nreduce(&da, -1, NULL) == NULL);
}
