#include <assert.h>

#include "da_tests.h"

NO_COV void test_cm_da_shrink(void) {
	t_cm_da da;

	da_init(&da, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), C_MINI__DA_NB_ELEM_DFLT, 0)
	alloc_set_call_limit(0);
	assert(da_shrink(&da) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI__DA_NB_ELEM_DFLT, 0)
	assert(da_shrink(&da) == true);
	assert(da_shrink(&da) == true);
	assert(da_shrink(&da) == true);
	assert(da_shrink(&da) == true);
	ASSERT_DA_STATE(da, sizeof(int), 0, 0)
	da_destroy(&da);
	assert(da_shrink(NULL) == false);
}
NO_COV void test_cm_da_shrink_min(void) {
	t_cm_da da;

	da_init(&da, sizeof(int));
	ASSERT_DA_STATE(da, sizeof(int), C_MINI__DA_NB_ELEM_DFLT, 0)
	alloc_set_call_limit(0);
	assert(da_shrink_min(&da, 1) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI__DA_NB_ELEM_DFLT, 0)
	assert(da_shrink_min(&da, C_MINI__DA_NB_ELEM_DFLT + 2) == true);
	ASSERT_DA_STATE(da, sizeof(int), C_MINI__DA_NB_ELEM_DFLT, 0)
	da_grow(&da);
	da_grow(&da);
	assert(da_shrink_min(&da, 5) == true);
	ASSERT_DA_STATE(da, sizeof(int), 5, 0)
	da_destroy(&da);
	assert(da_shrink_min(NULL, 5) == false);
}
NO_COV void test_cm_da_shrink_auto(void) {
	t_cm_da da;

	da_ninit(&da, 8, sizeof(int));
	da.nb_e = 2;
	assert(da_shrink_auto(&da) == true);
#ifndef C_MINI__DA_AUTO_SHRINK
	ASSERT_DA_STATE(da, sizeof(int), 8, 2)
	da_destroy(&da);
	assert(da_shrink_auto(NULL) == true);
#else
	ASSERT_DA_STATE(da, sizeof(int), 4, 2)
	da.nb_e = 1;
	assert(da_shrink_auto(&da) == true);
	ASSERT_DA_STATE(da, sizeof(int), 2, 1)
	da.nb_e = 0;
	assert(da_shrink_auto(&da) == true);
	ASSERT_DA_STATE(da, sizeof(int), 1, 0)
	assert(da_shrink_auto(&da) == true);
	ASSERT_DA_STATE(da, sizeof(int), 1, 0)
	da_destroy(&da);

	da_ninit(&da, 8, sizeof(int));
	da.nb_e = 4;
	assert(da_shrink_auto(&da) == true);
	ASSERT_DA_STATE(da, sizeof(int), 8, 4)
	da_destroy(&da);

	da_ninit(&da, 8, sizeof(int));
	da.nb_e = 2;
	alloc_set_call_limit(0);
	assert(da_shrink_auto(&da) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	ASSERT_DA_STATE(da, sizeof(int), 8, 2)
	da_destroy(&da);

	assert(da_shrink_auto(NULL) == false);

	int out;
	int i;

	da_ninit(&da, 8, sizeof(int));
	da.nb_e = 8;
	for (i = 0; i < 6; i++)
		assert(da_pop(&da, &out) == true);
	ASSERT_DA_STATE(da, sizeof(int), 4, 2)
	for (i = 0; i < 2; i++)
		assert(da_pop(&da, &out) == true);
	ASSERT_DA_STATE(da, sizeof(int), 1, 0)
	assert(da_pop(&da, &out) == false);
	da_destroy(&da);

	da_ninit(&da, 8, sizeof(int));
	da.nb_e = 8;
	for (i = 0; i < 6; i++)
		assert(da_rm_order(&da, 0) == true);
	ASSERT_DA_STATE(da, sizeof(int), 4, 2)
	assert(da_rm_order(&da, 0) == true);
	ASSERT_DA_STATE(da, sizeof(int), 2, 1)
	assert(da_rm_order(&da, 0) == true);
	ASSERT_DA_STATE(da, sizeof(int), 1, 0)
	da_destroy(&da);

	da_ninit(&da, 8, sizeof(int));
	da.nb_e = 8;
	assert(da_rms_order(&da, 0, 6) == true);
	ASSERT_DA_STATE(da, sizeof(int), 4, 2)
	assert(da_rms_order(&da, 0, 99) == true);
	ASSERT_DA_STATE(da, sizeof(int), 2, 0)
	da_destroy(&da);

	da_ninit(&da, 8, sizeof(int));
	da.nb_e = 8;
	assert(da_rms(&da, 0, 6) == true);
	ASSERT_DA_STATE(da, sizeof(int), 4, 2)
	assert(da_rms(&da, 0, 99) == true);
	ASSERT_DA_STATE(da, sizeof(int), 2, 0)
	da_destroy(&da);

	da_ninit(&da, 8, sizeof(int));
	da.nb_e = 8;
	for (i = 0; i < 6; i++)
		assert(da_rm(&da, 0) == true);
	ASSERT_DA_STATE(da, sizeof(int), 4, 2)
	assert(da_rm(&da, 0) == true);
	ASSERT_DA_STATE(da, sizeof(int), 2, 1)
	da_destroy(&da);

	da_ninit(&da, 8, sizeof(int));
	da.nb_e = 8;
	assert(da_filter(&da, cb_false, NULL, NULL) == true);
	ASSERT_DA_STATE(da, sizeof(int), 1, 0)
	da_destroy(&da);

	da_ninit(&da, 8, sizeof(int));
	da.nb_e = 8;
	assert(da_nfilter(&da, 8, cb_false, NULL, NULL) == true);
	ASSERT_DA_STATE(da, sizeof(int), 1, 0)
	da_destroy(&da);

	da_ninit(&da, 8, sizeof(int));
	da.nb_e = 8;
	assert(da_filter_out(&da, cb_true, NULL, NULL) == true);
	ASSERT_DA_STATE(da, sizeof(int), 1, 0)
	da_destroy(&da);

	da_ninit(&da, 8, sizeof(int));
	da.nb_e = 8;
	assert(da_nfilter_out(&da, 8, cb_true, NULL, NULL) == true);
	ASSERT_DA_STATE(da, sizeof(int), 1, 0)
	da_destroy(&da);
#endif // !C_MINI__DA_AUTO_SHRINK
}
