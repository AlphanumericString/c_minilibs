#include "da_tests.h"

// find
void test_da_find(void) {
	t_da  da;
	void *p;
	int   value = 1;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

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

	da_destroy(&da);
}
void test_da_nfind(void) {
	t_da  da;
	void *p;
	int   value = 1;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

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
	value = 42;
	p     = da_nfind(&da, -1, cb_cmp, &value);
	assert(p == NULL);

	da_destroy(&da);
}
void test_da_find_get(void) {
	t_da da;
	int  out;
	int  value = 1;
	bool ret;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
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

	da_destroy(&da);
}
