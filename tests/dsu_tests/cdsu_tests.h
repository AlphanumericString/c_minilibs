#pragma once

#include <assert.h>

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "dsu/cdsu/compact_disjointed_set_union.h"

// create
void test_cdsu_new(void);
void test_cdsu_new_from_array(void);
void test_cdsu_init(void);
void test_cdsu_init_from_arrays(void);

// grow
void test_cdsu_grow(void);

// destroy
void test_cdsu_clr(void);
void test_cdsu_destroy(void);
void test_cdsu_free(void);

// getter
void test_cdsu_max_depth(void);
void test_cdsu_size(void);
void test_cdsu_nb_components(void);

// querry
void test_cdsu_is_related(void);
void test_cdsu_strict_is_related(void);
void test_cdsu_validate(void);

// links
void test_cdsu_union(void);
void test_cdsu_find(void);
void test_cdsu_strict_find(void);
void test_cdsu_compress_all(void);
