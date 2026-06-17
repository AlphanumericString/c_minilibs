#include "da_tests.h"

// grow
void test_da_grow(void) {
	t_da da;

	da_init(&da, sizeof(int));

	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_grow(&da);
	ASSERT_DA_STATE(da, sizeof(int),
	                C_MINI_DA__NB_ELEM_DFLT * C_MINI_DA__GROWTH, 0);

	da_destroy(&da);
}
void test_da_grow_for_size(void) {
	t_da da;

	da_init(&da, sizeof(int));

	ASSERT_DA_STATE(da, sizeof(int), C_MINI_DA__NB_ELEM_DFLT, 0);
	da_grow_for_size(&da, 20);
	assert(da.cap_e >= 20);
	assert(da.sz_e == sizeof(int));
	assert(da.nb_e == 0);
	assert(da.array != NULL);

	da_destroy(&da);
}
