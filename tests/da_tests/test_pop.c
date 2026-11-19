#include <assert.h>

#include "da_tests.h"

NO_COV void test_cm_da_pop(void) {
	t_cm_da da;
	int     out;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	assert(da_pop(&da, &out) == true);
	assert(out == 5);
	assert(da.nb_e == 4);
	assert(da_pop(&da, &out) == true);
	assert(out == 4);
	assert(da.nb_e == 3);
	assert(da_pop(&da, &out) == true);
	assert(da_pop(&da, &out) == true);
	assert(da_pop(&da, &out) == true);
	assert(out == 1);
	assert(da.nb_e == 0);
	assert(da_pop(&da, &out) == false);
	assert(da_pop(NULL, &out) == false);
	assert(da_pop(&da, NULL) == false);

	da_destroy(&da);
}
