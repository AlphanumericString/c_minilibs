#include <assert.h>

#include "hp_tests.h"

static int dummy_elems[4];

NO_COV void test_cm_hp_is_valid(void) {
	const t_cm_hp ok         = {.nb       = 2,
	                            .capacity = 4,
	                            .sz_e     = sizeof(int),
	                            .elems    = dummy_elems,
	                            .cmp      = cb_cmp_int};
	const t_cm_hp no_elems   = {.nb       = 2,
	                            .capacity = 4,
	                            .sz_e     = sizeof(int),
	                            .elems    = NULL,
	                            .cmp      = cb_cmp_int};
	const t_cm_hp no_cmp     = {.nb       = 2,
	                            .capacity = 4,
	                            .sz_e     = sizeof(int),
	                            .elems    = dummy_elems,
	                            .cmp      = NULL};
	const t_cm_hp no_sz      = {.nb       = 2,
	                            .capacity = 4,
	                            .sz_e     = 0,
	                            .elems    = dummy_elems,
	                            .cmp      = cb_cmp_int};
	const t_cm_hp nb_inf_cap = {.nb       = 5,
	                            .capacity = 4,
	                            .sz_e     = sizeof(int),
	                            .elems    = dummy_elems,
	                            .cmp      = cb_cmp_int};

	assert(hp_is_valid(&ok) == true);
	assert(hp_is_valid(&no_elems) == false);
	assert(hp_is_valid(&no_cmp) == false);
	assert(hp_is_valid(&no_sz) == false);
	assert(hp_is_valid(&nb_inf_cap) == false);
	assert(hp_is_valid(NULL) == false);
}
NO_COV void test_cm_hp_is_empty(void) {
	const t_cm_hp empty     = {.nb       = 0,
	                           .capacity = 4,
	                           .sz_e     = sizeof(int),
	                           .elems    = dummy_elems,
	                           .cmp      = cb_cmp_int};
	const t_cm_hp not_empty = {.nb       = 2,
	                           .capacity = 4,
	                           .sz_e     = sizeof(int),
	                           .elems    = dummy_elems,
	                           .cmp      = cb_cmp_int};

	assert(hp_is_empty(&empty) == true);
	assert(hp_is_empty(&not_empty) == false);
	assert(hp_is_empty(NULL) == false);
}
NO_COV void test_cm_hp_is_full(void) {
	const t_cm_hp full     = {.nb       = 4,
	                          .capacity = 4,
	                          .sz_e     = sizeof(int),
	                          .elems    = dummy_elems,
	                          .cmp      = cb_cmp_int};
	const t_cm_hp not_full = {.nb       = 2,
	                          .capacity = 4,
	                          .sz_e     = sizeof(int),
	                          .elems    = dummy_elems,
	                          .cmp      = cb_cmp_int};
	const t_cm_hp no_elems = {.nb       = 4,
	                          .capacity = 4,
	                          .sz_e     = sizeof(int),
	                          .elems    = NULL,
	                          .cmp      = cb_cmp_int};

	assert(hp_is_full(&full) == true);
	assert(hp_is_full(&not_full) == false);
	assert(hp_is_full(&no_elems) == false);
	assert(hp_is_full(NULL) == false);
}

NO_COV void test_cm_hp_min_is_valid(void) {
	int     min_ok[]  = {1, 2, 3, 4, 5};
	int     max_ok[]  = {5, 4, 3, 2, 1};
	int     corrupt[] = {2, 1, 3};
	t_cm_hp ok        = {.nb       = 5,
	                     .capacity = 5,
	                     .sz_e     = sizeof(int),
	                     .elems    = min_ok,
	                     .cmp      = cb_cmp_int};
	t_cm_hp is_max    = {.nb       = 5,
	                     .capacity = 5,
	                     .sz_e     = sizeof(int),
	                     .elems    = max_ok,
	                     .cmp      = cb_cmp_int};
	t_cm_hp bad       = {.nb       = 3,
	                     .capacity = 3,
	                     .sz_e     = sizeof(int),
	                     .elems    = corrupt,
	                     .cmp      = cb_cmp_int};
	t_cm_hp empty     = {.nb       = 0,
	                     .capacity = 5,
	                     .sz_e     = sizeof(int),
	                     .elems    = min_ok,
	                     .cmp      = cb_cmp_int};
	t_cm_hp one       = {.nb       = 1,
	                     .capacity = 5,
	                     .sz_e     = sizeof(int),
	                     .elems    = min_ok,
	                     .cmp      = cb_cmp_int};

	assert(hp_min_is_valid(&ok) == true);
	assert(hp_min_is_valid(&is_max) == false);
	assert(hp_min_is_valid(&bad) == false);
	assert(hp_min_is_valid(&empty) == true);
	assert(hp_min_is_valid(&one) == true);
	assert(hp_min_is_valid(NULL) == false);
}

NO_COV void test_cm_hp_max_is_valid(void) {
	int     min_ok[]  = {1, 2, 3, 4, 5};
	int     max_ok[]  = {5, 4, 3, 2, 1};
	int     corrupt[] = {2, 1, 3};
	t_cm_hp ok        = {.nb       = 5,
	                     .capacity = 5,
	                     .sz_e     = sizeof(int),
	                     .elems    = max_ok,
	                     .cmp      = cb_cmp_int};
	t_cm_hp is_min    = {.nb       = 5,
	                     .capacity = 5,
	                     .sz_e     = sizeof(int),
	                     .elems    = min_ok,
	                     .cmp      = cb_cmp_int};
	t_cm_hp bad       = {.nb       = 3,
	                     .capacity = 3,
	                     .sz_e     = sizeof(int),
	                     .elems    = corrupt,
	                     .cmp      = cb_cmp_int};
	t_cm_hp empty     = {.nb       = 0,
	                     .capacity = 5,
	                     .sz_e     = sizeof(int),
	                     .elems    = max_ok,
	                     .cmp      = cb_cmp_int};
	t_cm_hp one       = {.nb       = 1,
	                     .capacity = 5,
	                     .sz_e     = sizeof(int),
	                     .elems    = max_ok,
	                     .cmp      = cb_cmp_int};

	assert(hp_max_is_valid(&ok) == true);
	assert(hp_max_is_valid(&is_min) == false);
	assert(hp_max_is_valid(&bad) == false);
	assert(hp_max_is_valid(&empty) == true);
	assert(hp_max_is_valid(&one) == true);
	assert(hp_max_is_valid(NULL) == false);
}
