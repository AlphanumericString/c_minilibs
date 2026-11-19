#include <assert.h>

#include "da_tests.h"

NO_COV void test_cm_da_size(void) {
	t_cm_da da;
	int     a;

	da_init(&da, sizeof(int));
	assert(da_size(&da) == 0);
	a = 1;
	da_add(&da, &a);
	assert(da_size(&da) == 1);
	da_destroy(&da);
	assert(da_size(NULL) == 0);
}

NO_COV void test_cm_da_capacity(void) {
	t_cm_da da;

	da_init(&da, sizeof(int));
	assert(da_capacity(&da) == C_MINI__DA_NB_ELEM_DFLT);
	da_destroy(&da);
	assert(da_capacity(NULL) == 0);
}

NO_COV void test_cm_da_front(void) {
	t_cm_da da;
	t_cm_da invalid = {0};

	da_init(&da, sizeof(int));
	assert(da_front(&da) == NULL);
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	assert(*(int *)da_front(&da) == arr_5[0]);
	da_destroy(&da);

	assert(da_front(NULL) == NULL);
	assert(da_front(&invalid) == NULL);
}

NO_COV void test_cm_da_back(void) {
	t_cm_da da;
	t_cm_da invalid = {0};

	da_init(&da, sizeof(int));
	assert(da_back(&da) == NULL);
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	assert(*(int *)da_back(&da) == arr_5[4]);
	da_destroy(&da);

	assert(da_back(NULL) == NULL);
	assert(da_back(&invalid) == NULL);
}

NO_COV void test_cm_da_reserve(void) {
	t_cm_da da;
	t_cm_da invalid = {0};
	size_t  cap;
	void   *array;

	da_init(&da, sizeof(int));
	assert(da_reserve(&da, 0) == true);
	assert(da_reserve(&da, 2) == true);
	assert(da_capacity(&da) == C_MINI__DA_NB_ELEM_DFLT);
	assert(da_reserve(&da, 20) == true);
	assert(da_capacity(&da) >= 20);
	assert(da.nb_e == 0);

	cap   = da.cap_e;
	array = da.array;
	alloc_set_call_limit(0);
	assert(da_reserve(&da, 1000) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(da.cap_e == cap);
	assert(da.array == array);

	assert(da_reserve(&da, SIZE_MAX) == false);
	da_destroy(&da);

	assert(da_reserve(NULL, 10) == false);
	assert(da_reserve(&invalid, 10) == false);
}

NO_COV void test_cm_da_resize(void) {
	t_cm_da da;
	t_cm_da invalid = {0};
	int     fill    = 7;
	size_t  i;

	da_init(&da, sizeof(int));
	assert(da_resize(&da, 2, &fill) == true);
	assert(da.nb_e == 2);
	assert(*(int *)da_at(&da, 0) == fill);
	assert(*(int *)da_at(&da, 1) == fill);

	assert(da_resize(&da, 4, NULL) == true);
	assert(da.nb_e == 4);
	assert(*(int *)da_at(&da, 2) == 0);
	assert(*(int *)da_at(&da, 3) == 0);

	assert(da_resize(&da, 8, &fill) == true);
	assert(da.nb_e == 8);
	assert(da.cap_e >= 8);
	for (i = 4; i < 8; i++)
		assert(*(int *)da_at(&da, i) == fill);

	assert(da_resize(&da, 3, &fill) == true);
	assert(da.nb_e == 3);
	assert(da_resize(&da, 3, &fill) == true);
	assert(da.nb_e == 3);

	alloc_set_call_limit(0);
	assert(da_resize(&da, 1000, &fill) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(da.nb_e == 3);

	assert(da_resize(&da, SIZE_MAX, NULL) == false);
	da_destroy(&da);

	assert(da_resize(NULL, 2, &fill) == false);
	assert(da_resize(&invalid, 2, &fill) == false);
}

NO_COV void test_cm_da_insert(void) {
	t_cm_da da;
	t_cm_da invalid = {0};
	int     vals[]  = {1, 2, 3, 4, 5};

	da_init(&da, sizeof(int));
	assert(da_insert(&da, 0, &vals[0]) == true);
	assert(da.nb_e == 1);
	assert(*(int *)da_at(&da, 0) == vals[0]);
	assert(da_insert(&da, 1, &vals[1]) == true);
	assert(*(int *)da_at(&da, 1) == vals[1]);
	assert(da_insert(&da, 0, &vals[2]) == true);
	assert(*(int *)da_at(&da, 0) == vals[2]);
	assert(*(int *)da_at(&da, 1) == vals[0]);
	assert(*(int *)da_at(&da, 2) == vals[1]);
	assert(da_insert(&da, 1, &vals[3]) == true);
	assert(*(int *)da_at(&da, 1) == vals[3]);
	assert(*(int *)da_at(&da, 2) == vals[0]);
	assert(da.nb_e == 4);

	assert(da_insert(&da, 4, &vals[4]) == true);
	assert(da.nb_e == 5);
	assert(*(int *)da_at(&da, 4) == vals[4]);
	da_destroy(&da);

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_2_5, 4, sizeof(int));
	alloc_set_call_limit(0);
	assert(da_insert(&da, 2, &vals[0]) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(da.nb_e == 4);
	da_destroy(&da);

	assert(da_insert(NULL, 0, &vals[0]) == false);
	assert(da_insert(&da, 0, &vals[0]) == false);
	assert(da_insert(&invalid, 0, &vals[0]) == false);
	da_init(&da, sizeof(int));
	assert(da_insert(&da, 1, &vals[0]) == false);
	assert(da_insert(&da, 0, NULL) == false);
	da_destroy(&da);
}

NO_COV void test_cm_da_erase(void) {
	t_cm_da da;
	t_cm_da invalid = {0};

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));
	assert(da_erase(&da, 1) == true);
	assert(da.nb_e == 4);
	assert(*(int *)da_at(&da, 0) == 1);
	assert(*(int *)da_at(&da, 1) == 3);
	assert(*(int *)da_at(&da, 2) == 4);
	assert(*(int *)da_at(&da, 3) == 5);
	assert(da_erase(&da, 0) == true);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(da_erase(&da, 2) == true);
	assert(da.nb_e == 2);
	assert(*(int *)da_at(&da, 0) == 3);
	assert(*(int *)da_at(&da, 1) == 4);
	assert(da_erase(&da, 2) == false);
	assert(da.nb_e == 2);
	da_destroy(&da);

	assert(da_erase(NULL, 0) == false);
	assert(da_erase(&invalid, 0) == false);
}
