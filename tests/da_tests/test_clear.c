#include <assert.h>

#include "da_tests.h"

NO_COV void test_da_clr(void) {
	t_da   da;
	size_t sz;
	size_t cap;
	void  *arr;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_3, 3, sizeof(int));
	sz  = da.sz_e;
	cap = da.cap_e;
	arr = da.array;
	da_clr(&da);
	ASSERT_DA_STATE(da, sz, cap, 0)
	assert(da.array == arr);
	da_destroy(&da);
	da_clr(NULL);
}
