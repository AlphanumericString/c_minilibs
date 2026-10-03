#include <assert.h>
#include <stdlib.h>

#include "da_tests.h"

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
	da_destroy(&da);
	da_destroy(NULL);
}

void test_da_free(void) {
	t_da *da;

	da = da_new(sizeof(int));
	da_free(da);
	da  = malloc(sizeof(t_da));
	*da = (t_da){0};
	da_free(da); // check valgrind for no leaks
	da_free(NULL);
}
