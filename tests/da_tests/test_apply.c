#include <assert.h>

#include "da_tests.h"

void test_da_apply(void) {
	t_da da;
	int  a = 1;

	da_init(&da, sizeof(int));
	da_add(&da, &a);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(da_apply(&da, cb_true, NULL) == NULL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(da_apply(&da, cb_true, (void *)0xBEEFDEAD) == (void *)0xBEEFDEAD);
	assert(da_apply(&da, cb_add_one, (void *)0xDEAD) == (void *)0xDEAD);
	assert(*(int *)da_at(&da, 0) == 2);

	da_destroy(&da);
}
void test_da_napply(void) {
	t_da da;
	int  a = 1;

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	assert(*(int *)da_at(&da, 0) == 1);
	assert(da_napply(&da, 1, cb_add_one, NULL) == NULL);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(da_napply(&da, 14, cb_add_one, (void *)0xDEAD) == (void *)0xDEAD);
	assert(*(int *)da_at(&da, 0) == 3);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	da_napply(&da, 3, cb_add_one, NULL);
	assert(*(int *)da_at(&da, 0) == 4);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);

	da_destroy(&da);
}
void test_da_apply_it(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	da_apply_it(&da, cb_it_add_it_to_arg, (void *)3UL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 5);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);

	da_destroy(&da);
}
void test_da_napply_it(void) {
	t_da da;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	da_napply_it(&da, 1, cb_it_true, (void *)3UL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 2);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	da_napply_it(&da, 2, cb_it_add_it_to_arg, (void *)3UL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 3);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	da_napply_it(&da, 3, cb_it_add_it_to_arg, (void *)3UL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(*(int *)da_at(&da, 2) == 5);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	da_napply_it(&da, 4, cb_it_add_it_to_arg, (void *)3UL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 5);
	assert(*(int *)da_at(&da, 2) == 7);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);

	da_destroy(&da);
}
