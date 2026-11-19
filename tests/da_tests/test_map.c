#include <assert.h>

#include "da_tests.h"

NO_COV void test_cm_da_map(void) {
	t_cm_da da;
	t_cm_da out;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	out = da_map(&da, cb_map_double, sizeof(int));
	ASSERT_DA_STATE(out, sizeof(int), 5, 5)
	assert(*(int *)da_at(&out, 0) == 2);
	assert(*(int *)da_at(&out, 1) == 4);
	assert(*(int *)da_at(&out, 2) == 6);
	assert(*(int *)da_at(&out, 3) == 8);
	assert(*(int *)da_at(&out, 4) == 10);
	da_destroy(&out);

	out = da_map(&da, cb_map_to_long, sizeof(long));
	ASSERT_DA_STATE(out, sizeof(long), 5, 5)
	assert(*(long *)da_at(&out, 0) == 10L);
	assert(*(long *)da_at(&out, 1) == 20L);
	assert(*(long *)da_at(&out, 2) == 30L);
	assert(*(long *)da_at(&out, 3) == 40L);
	assert(*(long *)da_at(&out, 4) == 50L);
	da_destroy(&out);

	out = da_map(&da, cb_map_fail_on_3, sizeof(int));
	assert(out.array == NULL && out.cap_e == 0 && out.nb_e == 0 &&
	       out.sz_e == 0);
	out = da_map(&da, cb_map_null, sizeof(int));
	assert(out.array == NULL && out.cap_e == 0 && out.nb_e == 0 &&
	       out.sz_e == 0);

	alloc_set_call_limit(0);
	out = da_map(&da, cb_map_double, sizeof(int));
	assert(out.array == NULL && out.cap_e == 0 && out.nb_e == 0 &&
	       out.sz_e == 0);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	out = da_map(NULL, cb_map_double, sizeof(int));
	assert(out.array == NULL && out.cap_e == 0);
	out = da_map(&da, NULL, sizeof(int));
	assert(out.array == NULL && out.cap_e == 0);
	out = da_map(&da, cb_map_double, 0);
	assert(out.array == NULL && out.cap_e == 0);

	da_clr(&da);
	out = da_map(&da, cb_map_double, sizeof(int));
	assert(out.array != NULL);
	assert(out.nb_e == 0);
	assert(out.cap_e == C_MINI__DA_NB_ELEM_DFLT);
	assert(out.sz_e == sizeof(int));
	da_destroy(&out);

	da_destroy(&da);
}

NO_COV void test_cm_da_nmap(void) {
	t_cm_da da;
	t_cm_da out;

	da_init(&da, sizeof(int));
	da_cat_array(&da, (int *)arr_5, 5, sizeof(int));

	out = da_nmap(&da, 2, cb_map_double, sizeof(int));
	ASSERT_DA_STATE(out, sizeof(int), 2, 2)
	assert(*(int *)da_at(&out, 0) == 2);
	assert(*(int *)da_at(&out, 1) == 4);
	da_destroy(&out);

	out = da_nmap(&da, 99, cb_map_double, sizeof(int));
	ASSERT_DA_STATE(out, sizeof(int), 5, 5)
	assert(*(int *)da_at(&out, 4) == 10);
	da_destroy(&out);

	out = da_nmap(&da, 3, cb_map_fail_on_3, sizeof(int));
	assert(out.array == NULL && out.cap_e == 0 && out.nb_e == 0 &&
	       out.sz_e == 0);

	out = da_nmap(&da, 0, cb_map_double, sizeof(int));
	assert(out.array == NULL);
	out = da_nmap(NULL, 2, cb_map_double, sizeof(int));
	assert(out.array == NULL);
	out = da_nmap(&da, 2, NULL, sizeof(int));
	assert(out.array == NULL);
	out = da_nmap(&da, 2, cb_map_double, 0);
	assert(out.array == NULL);

	da_destroy(&da);
}
