#include <assert.h>

#include "da_tests.h"

NO_COV void test_cm_da_iter(void) {
	t_cm_da da;
	void   *it_val;
	size_t  start;
	size_t  count;
	int     sum;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	it_val = NULL;
	start  = 0;
	count  = 0;
	sum    = 0;
	while (da_iter(&da, start, (size_t)-1, &it_val)) {
		assert(it_val == da_at(&da, count));
		count++;
		sum += *(int *)it_val;
	}
	assert(count == 5);
	assert(sum == 15);
	assert(it_val == NULL);
	assert(da_iter(&da, start, (size_t)-1, &it_val) == true);
	assert(it_val == da_at(&da, start));

	it_val = NULL;
	count  = 0;
	sum    = 0;
	while (da_iter(&da, 1, 3, &it_val)) {
		count++;
		sum += *(int *)it_val;
	}
	assert(count == 2);
	assert(sum == 5);
	assert(it_val == NULL);

	it_val = NULL;
	count  = 0;
	while (da_iter(&da, 3, 99, &it_val))
		count++;
	assert(count == 2);

	it_val = (void *)0x1;
	assert(da_iter(&da, 2, 2, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(da_iter(&da, 5, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(da_iter(&da, 3, 1, &it_val) == false);
	assert(it_val == NULL);
	it_val = (void *)0x1;
	assert(da_iter(NULL, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(da_iter(&da, 0, (size_t)-1, NULL) == false);

	da_clr(&da);
	it_val = (void *)0x1;
	assert(da_iter(&da, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);

	da_destroy(&da);
}

NO_COV void test_cm_da_iter_rev(void) {
	t_cm_da da;
	void   *it_val;
	size_t  start;
	size_t  count;
	int     sum;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	it_val = NULL;
	start  = 0;
	count  = 0;
	sum    = 0;
	while (da_iter_rev(&da, start, (size_t)-1, &it_val)) {
		assert(it_val == da_at(&da, da.nb_e - 1 - count));
		count++;
		sum += *(int *)it_val;
	}
	assert(count == 5);
	assert(sum == 15);
	assert(it_val == NULL);
	assert(da_iter_rev(&da, start, (size_t)-1, &it_val) == true);
	assert(it_val == da_at(&da, da.nb_e - 1));

	it_val = NULL;
	count  = 0;
	sum    = 0;
	while (da_iter_rev(&da, 1, 3, &it_val)) {
		count++;
		sum += *(int *)it_val;
	}
	assert(count == 2);
	assert(sum == 5);
	assert(it_val == NULL);

	it_val = NULL;
	count  = 0;
	while (da_iter_rev(&da, 3, 99, &it_val))
		count++;
	assert(count == 2);

	it_val = (void *)0x1;
	assert(da_iter_rev(&da, 2, 2, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(da_iter_rev(&da, 5, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(da_iter_rev(&da, 3, 1, &it_val) == false);
	assert(it_val == NULL);
	it_val = (void *)0x1;
	assert(da_iter_rev(NULL, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(da_iter_rev(&da, 0, (size_t)-1, NULL) == false);

	da_clr(&da);
	it_val = (void *)0x1;
	assert(da_iter_rev(&da, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);

	da_destroy(&da);
}

NO_COV void test_cm_da_it_advance(void) {
	t_cm_da da;
	void   *it;
	int    *typed;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	it = NULL;
	assert(da_it_advance(&da, &it) == da_at(&da, 0));
	assert(da_it_advance(&da, &it) == da_at(&da, 1));
	assert(da_it_advance(&da, &it) == da_at(&da, 2));
	assert(da_it_advance(&da, &it) == da_at(&da, 3));
	assert(da_it_advance(&da, &it) == da_at(&da, 4));
	assert(da_it_advance(&da, &it) == NULL);
	assert(it == NULL);
	assert(da_it_advance(&da, &it) == da_at(&da, 0));

	typed = NULL;
	assert(da_it_advance(&da, (void **)&typed) == da_at(&da, 0));
	assert(*typed == 1);
	assert(da_it_advance(&da, (void **)&typed) == da_at(&da, 1));
	assert(*typed == 2);

	it = (void *)0x1;
	assert(da_it_advance(NULL, &it) == NULL);
	assert(it == NULL);
	assert(da_it_advance(&da, NULL) == NULL);

	da_clr(&da);
	it = (void *)0x1;
	assert(da_it_advance(&da, &it) == NULL);
	assert(it == NULL);

	da_destroy(&da);
}

NO_COV void test_cm_da_it_back(void) {
	t_cm_da da;
	void   *it;
	int    *typed;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	it = NULL;
	assert(da_it_back(&da, &it) == da_at(&da, 4));
	assert(da_it_back(&da, &it) == da_at(&da, 3));
	assert(da_it_back(&da, &it) == da_at(&da, 2));
	assert(da_it_back(&da, &it) == da_at(&da, 1));
	assert(da_it_back(&da, &it) == da_at(&da, 0));
	assert(da_it_back(&da, &it) == NULL);
	assert(it == NULL);
	assert(da_it_back(&da, &it) == da_at(&da, 4));

	typed = NULL;
	assert(da_it_back(&da, (void **)&typed) == da_at(&da, 4));
	assert(*typed == 5);
	assert(da_it_back(&da, (void **)&typed) == da_at(&da, 3));
	assert(*typed == 4);

	it = (void *)0x1;
	assert(da_it_back(NULL, &it) == NULL);
	assert(it == NULL);
	assert(da_it_back(&da, NULL) == NULL);

	da_clr(&da);
	it = (void *)0x1;
	assert(da_it_back(&da, &it) == NULL);
	assert(it == NULL);

	da_destroy(&da);
}
