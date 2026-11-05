#include <assert.h>

#include "da_tests.h"

NO_COV void test_cm_da_apply(void) {
	t_cm_da da;
	int     a    = 1;
	void   *data = (void *)0xDEAD;

	da_init(&da, sizeof(int));
	da_add(&da, &a);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(da_apply(&da, cb_true, NULL) == NULL);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(da_apply(&da, cb_true, data) == data);
	assert(da_apply(&da, cb_add_one, data) == data);
	assert(*(int *)da_at(&da, 0) == 2);
	da_destroy(&da);

	assert(da_apply(NULL, cb_add_one, data) == data);
	da = (t_cm_da){.array = data, .cap_e = 12, .sz_e = 1, .nb_e = 12};
	assert(da_apply(&da, NULL, data) == data);
	da = (t_cm_da){.array = data, .cap_e = 12, .sz_e = 1, .nb_e = 0};
	assert(da_apply(&da, cb_add_one, data) == data);
}
NO_COV void test_cm_da_napply(void) {
	t_cm_da da;
	int     a    = 1;
	void   *data = (void *)0xDEAD;

	da_init(&da, sizeof(int));
	da_add(&da, &a);

	assert(*(int *)da_at(&da, 0) == 1);
	assert(da_napply(&da, 1, cb_add_one, NULL) == NULL);
	assert(*(int *)da_at(&da, 0) == 2);
	assert(da_napply(&da, 14, cb_add_one, data) == data);
	assert(*(int *)da_at(&da, 0) == 3);
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	assert(da_napply(&da, 3, cb_add_one, NULL) == NULL);
	assert(*(int *)da_at(&da, 0) == 4);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);
	assert(da_napply(&da, 1, cb_false, data) == data);
	da_destroy(&da);

	assert(da_napply(NULL, 3, cb_add_one, data) == data);
	da = (t_cm_da){.array = NULL, .cap_e = 12, .nb_e = 12, .sz_e = 2};
	assert(da_napply(&da, 3, cb_add_one, data) == data);
	da = (t_cm_da){.array = (void *)0xCAFE, .cap_e = 12, .nb_e = 0, .sz_e = 2};
	assert(da_napply(&da, 3, cb_add_one, data) == data);
	da = (t_cm_da){.array = (void *)0xCAFE, .cap_e = 12, .nb_e = 12, .sz_e = 2};
	assert(da_napply(&da, 0, cb_add_one, data) == data);
	assert(da_napply(&da, 12, NULL, data) == data);
}
NO_COV void test_cm_da_apply_it(void) {
	t_cm_da da;

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
	da_apply_it(NULL, cb_it_add_it_to_arg, (void *)3UL);
	da = (t_cm_da){.array = (void *)0xCAFE, .cap_e = 12, .nb_e = 0, .sz_e = 2};
	da_apply_it(&da, cb_it_add_it_to_arg, (void *)3UL);
	da = (t_cm_da){.array = (void *)0xCAFE, .cap_e = 12, .nb_e = 12, .sz_e = 2};
	da_apply_it(&da, NULL, (void *)3UL);
}
NO_COV void test_cm_da_napply_it(void) {
	t_cm_da da;

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

	da_napply_it(NULL, 1, cb_it_add_it_to_arg, (void *)3UL);
	da = (t_cm_da){.array = (void *)0xCAFE, .cap_e = 12, .nb_e = 0, .sz_e = 2};
	da_napply_it(&da, 1, cb_it_add_it_to_arg, (void *)3UL);
	da = (t_cm_da){.array = (void *)0xCAFE, .cap_e = 12, .nb_e = 12, .sz_e = 2};
	da_napply_it(&da, 1, NULL, (void *)3UL);
	da_napply_it(&da, 0, cb_it_add_it_to_arg, (void *)3UL);
}
