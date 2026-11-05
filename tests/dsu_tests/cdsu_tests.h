#pragma once

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "common_base/tests.h"
#include "dsu/cdsu/compact_disjointed_set_union.h"

void test_cm_cdsu_new(void);
void test_cm_cdsu_new_from_array(void);
void test_cm_cdsu_init(void);
void test_cm_cdsu_init_from_arrays(void);

void test_cm_cdsu_grow(void);

void test_cm_cdsu_clr(void);
void test_cm_cdsu_destroy(void);
void test_cm_cdsu_free(void);

void test_cm_cdsu_max_depth(void);
void test_cm_cdsu_size(void);
void test_cm_cdsu_nb_components(void);

void test_cm_cdsu_is_related(void);
void test_cm_cdsu_strict_is_related(void);
void test_cm_cdsu_validate(void);

void test_cm_cdsu_union(void);
void test_cm_cdsu_find(void);
void test_cm_cdsu_strict_find(void);
void test_cm_cdsu_compress_all(void);
