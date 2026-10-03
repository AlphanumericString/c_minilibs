#include <assert.h>

#include "da_tests.h"

void test_da_find(void) {
	t_da da = {
	    .array = (void *)arr_5,
	    .cap_e = 5,
	    .nb_e  = 5,
	    .sz_e  = sizeof(int),
	};
	void *p;
	int   value = 1;

	p = da_find(&da, cb_cmp, &value);
	assert(p != NULL);
	assert(*(int *)p == 1);
	assert(p == da_at(&da, 0));
	value = 3;
	p     = da_find(&da, cb_cmp, &value);
	assert(p != NULL);
	assert(*(int *)p == 3);
	assert(p == da_at(&da, 2));
	value = 42;
	p     = da_find(&da, cb_cmp, &value);
	assert(p == NULL);
	assert(da_find(&da, NULL, &value) == NULL);
	assert(da_find(NULL, cb_cmp, &value) == NULL);
}
void test_da_nfind(void) {
	t_da da = {
	    .array = (void *)arr_5,
	    .cap_e = 5,
	    .nb_e  = 5,
	    .sz_e  = sizeof(int),
	};
	void *p;
	int   value = 1;

	p = da_nfind(&da, 1, cb_cmp, &value);
	assert(p != NULL);
	assert(*(int *)p == 1);
	assert(p == da_at(&da, 0));
	value = 3;
	p     = da_nfind(&da, 1, cb_cmp, &value);
	assert(p == NULL);
	p = da_nfind(&da, 5, cb_cmp, &value);
	assert(p != NULL);
	assert(*(int *)p == 3);
	assert(p == da_at(&da, 2));
	p = da_nfind(&da, 0, cb_cmp, &value);
	assert(p == NULL);
	value = 42;
	p     = da_nfind(&da, -1, cb_cmp, &value);
	assert(p == NULL);

	assert(da_nfind(&da, -1, NULL, &value) == NULL);
	assert(da_nfind(NULL, -1, cb_cmp, &value) == NULL);
}
void test_da_find_get(void) {
	t_da da = {
	    .array = (void *)arr_5,
	    .cap_e = 5,
	    .nb_e  = 5,
	    .sz_e  = sizeof(int),
	};
	int  out;
	int  value = 1;
	bool ret;

	ret = da_find_get(&da, cb_cmp, &value, &out);
	assert(ret == true);
	assert(out == 1);
	value = 3;
	ret   = da_find_get(&da, cb_cmp, &value, &out);
	assert(ret == true);
	assert(out == 3);
	value = 4;
	ret   = da_find_get(&da, cb_cmp, &value, &out);
	assert(ret == true);
	assert(out == 4);
	value = 42;
	out   = -3012;
	ret   = da_find_get(&da, cb_cmp, &value, &out);
	assert(ret == false);
	assert(out == -3012);
	value = 2;

	ret = da_find_get(NULL, cb_cmp, &value, &out);
	assert(ret == false);
	ret = da_find_get(&da, NULL, &value, &out);
	assert(ret == false);
	ret = da_find_get(&da, cb_cmp, &value, NULL);
	assert(ret == false);
}
