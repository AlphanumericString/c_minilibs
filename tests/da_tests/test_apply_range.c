#include <assert.h>

#include "da_tests.h"

NO_COV void test_cm_da_apply_range(void) {
	t_cm_da          da;
	t_da_range_state st;
	size_t           stop;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	st = (t_da_range_state){0};
	assert(da_apply_range(&da, 0, -1, cb_it_record, &st) == (void *)&st);
	assert(st.count == 5);
	assert(st.first_pos == 0);
	assert(st.last_pos == 4);
	assert(st.nb_last == 1);
	assert(st.sum == 15);

	st = (t_da_range_state){0};
	assert(da_apply_range(&da, 1, 3, cb_it_record, &st) == (void *)&st);
	assert(st.count == 2);
	assert(st.first_pos == 1);
	assert(st.last_pos == 2);
	assert(st.nb_last == 1);
	assert(st.sum == 5);

	st = (t_da_range_state){0};
	assert(da_apply_range(&da, 2, 5, cb_it_record, &st) == (void *)&st);
	assert(st.count == 3);
	assert(st.first_pos == 2);
	assert(st.last_pos == 4);
	assert(st.nb_last == 1);
	assert(st.sum == 12);

	st = (t_da_range_state){0};
	assert(da_apply_range(&da, 3, 99, cb_it_record, &st) == (void *)&st);
	assert(st.count == 2);
	assert(st.first_pos == 3);
	assert(st.last_pos == 4);
	assert(st.nb_last == 1);
	assert(st.sum == 9);

	st = (t_da_range_state){0};
	assert(da_apply_range(&da, 2, 2, cb_it_record, &st) == (void *)&st);
	assert(st.count == 0);

	stop = 2;
	assert(da_apply_range(&da, 0, -1, cb_it_stop, &stop) == (void *)&stop);
	assert(*(int *)da_at(&da, 0) == 101);
	assert(*(int *)da_at(&da, 1) == 102);
	assert(*(int *)da_at(&da, 2) == 103);
	assert(*(int *)da_at(&da, 3) == 4);
	assert(*(int *)da_at(&da, 4) == 5);

	st = (t_da_range_state){0};
	assert(da_apply_range(NULL, 0, -1, cb_it_record, &st) == (void *)&st);
	assert(st.count == 0);

	st = (t_da_range_state){0};
	assert(da_apply_range(&da, 5, -1, cb_it_record, &st) == (void *)&st);
	assert(st.count == 0);

	st = (t_da_range_state){0};
	assert(da_apply_range(&da, 3, 1, cb_it_record, &st) == (void *)&st);
	assert(st.count == 0);

	st = (t_da_range_state){0};
	assert(da_apply_range(&da, 0, -1, NULL, &st) == (void *)&st);
	assert(st.count == 0);

	da_clr(&da);
	st = (t_da_range_state){0};
	assert(da_apply_range(&da, 0, -1, cb_it_record, &st) == (void *)&st);
	assert(st.count == 0);

	da_destroy(&da);
}
