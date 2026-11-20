#include <assert.h>

#include "hp_tests.h"

static void build_min(t_cm_hp *h) {
	int vals[] = {1, 2, 3, 4, 5};

	for (size_t i = 0; i < 5; i++)
		assert(hp_min_push(h, &vals[i]) == true);
}

NO_COV void test_cm_hp_iter(void) {
	t_cm_hp h;
	void   *it_val;
	size_t  count;
	int     sum;

	assert(hp_init(&h, 5, sizeof(int), cb_cmp_int) == &h);
	build_min(&h);

	it_val = NULL;
	count  = 0;
	sum    = 0;
	while (hp_iter(&h, 0, (size_t)-1, &it_val)) {
		assert(it_val == hp_at(&h, count));
		count++;
		sum += *(int *)it_val;
	}
	assert(count == 5);
	assert(sum == 15);
	assert(it_val == NULL);
	assert(hp_iter(&h, 0, (size_t)-1, &it_val) == true);
	assert(it_val == hp_at(&h, 0));

	it_val = NULL;
	count  = 0;
	sum    = 0;
	while (hp_iter(&h, 1, 3, &it_val)) {
		assert(it_val == hp_at(&h, count + 1));
		count++;
		sum += *(int *)it_val;
	}
	assert(count == 2);
	assert(sum == 5);
	assert(it_val == NULL);

	it_val = NULL;
	count  = 0;
	while (hp_iter(&h, 3, 99, &it_val))
		count++;
	assert(count == 2);

	it_val = (void *)0x1;
	assert(hp_iter(&h, 2, 2, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(hp_iter(&h, 5, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(hp_iter(&h, 3, 1, &it_val) == false);
	assert(it_val == NULL);
	it_val = (void *)0x1;
	assert(hp_iter(NULL, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(hp_iter(&h, 0, (size_t)-1, NULL) == false);

	hp_clear(&h);
	it_val = (void *)0x1;
	assert(hp_iter(&h, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);

	hp_delete(&h);
}

NO_COV void test_cm_hp_iter_rev(void) {
	t_cm_hp h;
	void   *it_val;
	size_t  count;
	int     sum;

	assert(hp_init(&h, 5, sizeof(int), cb_cmp_int) == &h);
	build_min(&h);

	it_val = NULL;
	count  = 0;
	sum    = 0;
	while (hp_iter_rev(&h, 0, (size_t)-1, &it_val)) {
		assert(it_val == hp_at(&h, h.nb - 1 - count));
		count++;
		sum += *(int *)it_val;
	}
	assert(count == 5);
	assert(sum == 15);
	assert(it_val == NULL);
	assert(hp_iter_rev(&h, 0, (size_t)-1, &it_val) == true);
	assert(it_val == hp_at(&h, h.nb - 1));

	it_val = NULL;
	count  = 0;
	while (hp_iter_rev(&h, 1, 3, &it_val)) {
		assert(it_val == hp_at(&h, 2 - count));
		count++;
	}
	assert(count == 2);
	assert(it_val == NULL);

	it_val = NULL;
	count  = 0;
	while (hp_iter_rev(&h, 3, 99, &it_val))
		count++;
	assert(count == 2);

	it_val = (void *)0x1;
	assert(hp_iter_rev(&h, 2, 2, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(hp_iter_rev(&h, 5, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(hp_iter_rev(&h, 3, 1, &it_val) == false);
	assert(it_val == NULL);
	it_val = (void *)0x1;
	assert(hp_iter_rev(NULL, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);
	it_val = NULL;
	assert(hp_iter_rev(&h, 0, (size_t)-1, NULL) == false);

	hp_clear(&h);
	it_val = (void *)0x1;
	assert(hp_iter_rev(&h, 0, (size_t)-1, &it_val) == false);
	assert(it_val == NULL);

	hp_delete(&h);
}

NO_COV void test_cm_hp_it_advance(void) {
	t_cm_hp h;
	void   *it;
	int    *typed;

	assert(hp_init(&h, 5, sizeof(int), cb_cmp_int) == &h);
	build_min(&h);

	it = NULL;
	assert(hp_it_advance(&h, &it) == hp_at(&h, 0));
	assert(hp_it_advance(&h, &it) == hp_at(&h, 1));
	assert(hp_it_advance(&h, &it) == hp_at(&h, 2));
	assert(hp_it_advance(&h, &it) == hp_at(&h, 3));
	assert(hp_it_advance(&h, &it) == hp_at(&h, 4));
	assert(hp_it_advance(&h, &it) == NULL);
	assert(it == NULL);
	assert(hp_it_advance(&h, &it) == hp_at(&h, 0));

	typed = NULL;
	assert(hp_it_advance(&h, (void **)&typed) == hp_at(&h, 0));
	assert(*typed == 1);
	assert(hp_it_advance(&h, (void **)&typed) == hp_at(&h, 1));
	assert(*typed == 2);

	it = (void *)0x1;
	assert(hp_it_advance(NULL, &it) == NULL);
	assert(it == NULL);
	assert(hp_it_advance(&h, NULL) == NULL);

	hp_clear(&h);
	it = (void *)0x1;
	assert(hp_it_advance(&h, &it) == NULL);
	assert(it == NULL);

	hp_delete(&h);
}

NO_COV void test_cm_hp_it_back(void) {
	t_cm_hp h;
	void   *it;
	int    *typed;

	assert(hp_init(&h, 5, sizeof(int), cb_cmp_int) == &h);
	build_min(&h);

	it = NULL;
	assert(hp_it_back(&h, &it) == hp_at(&h, 4));
	assert(hp_it_back(&h, &it) == hp_at(&h, 3));
	assert(hp_it_back(&h, &it) == hp_at(&h, 2));
	assert(hp_it_back(&h, &it) == hp_at(&h, 1));
	assert(hp_it_back(&h, &it) == hp_at(&h, 0));
	assert(hp_it_back(&h, &it) == NULL);
	assert(it == NULL);
	assert(hp_it_back(&h, &it) == hp_at(&h, 4));

	typed = NULL;
	assert(hp_it_back(&h, (void **)&typed) == hp_at(&h, 4));
	assert(*typed == 5);
	assert(hp_it_back(&h, (void **)&typed) == hp_at(&h, 3));
	assert(*typed == 4);

	it = (void *)0x1;
	assert(hp_it_back(NULL, &it) == NULL);
	assert(it == NULL);
	assert(hp_it_back(&h, NULL) == NULL);

	hp_clear(&h);
	it = (void *)0x1;
	assert(hp_it_back(&h, &it) == NULL);
	assert(it == NULL);

	hp_delete(&h);
}
