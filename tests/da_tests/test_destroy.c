#include "da_tests.h"

// destroy
void test_da_destroy(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	ASSERT_DA_STATE(da, da.sz_e, da.cap_e, 5);

	da_destroy(&da);
	assert(da.sz_e == 0);
	assert(da.cap_e == 0);
	assert(da.nb_e == 0);
	assert(da.array == NULL);
}
