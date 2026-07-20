#include <assert.h>

#include "da_tests.h"

void test_da_rm_order(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	da_rm_order(&da, 0);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	assert(*(int *)da_at(&da, 3) == 5);
	assert(da.nb_e == 4);
	da_rm_order(&da, 0);
	assert(da.nb_e == 3);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 5);
	assert((int *)da_at(&da, 3) == NULL);
	da_rm_order(&da, 1);
	assert(da.nb_e == 2);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 5);
	assert((int *)da_at(&da, 2) == NULL);
	da_rm_order(&da, -1);
	da_destroy(&da);
}
void test_da_rms_order(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	da_rms_order(&da, 0, 1);
	assert(da.nb_e == 4);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	assert(*(int *)da_at(&da, 3) == 5);
	da_rms_order(&da, 1, 3);
	assert(da.nb_e == 2);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(*(int *)da_at(&da, 1) == 5);
	da_rms_order(&da, 1, -1);
	assert(da.nb_e == 1);
	assert(*(int *)da_at(&da, 0) == 2);
	da_clr(&da);
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	da_rms_order(&da, 1, -1);
	assert(da.nb_e == 1);

	da_destroy(&da);
}
void test_da_rm(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	da_rm(&da, 0);
	assert(*(int *)da_at(&da, 0) != 1);
	assert(da.nb_e == 4);
	assert(*(int *)da_at(&da, 0) == 5);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	da_rm(&da, 1);
	assert(da.nb_e == 3);
	assert(*(int *)da_at(&da, 0) == 5);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 3);

	da_destroy(&da);
}
// TODO: better tests when fixing da_rms
void test_da_rms(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	da_rms(&da, 0, 1);
	assert(da.nb_e == 4);
	assert(*(int *)da_at(&da, 0) != 1);
	da_rms(&da, 0, 2);
	assert(da.nb_e == 2);
	da_rms(&da, 0, -1);
	assert(da.nb_e == 0);

	da_destroy(&da);
}
