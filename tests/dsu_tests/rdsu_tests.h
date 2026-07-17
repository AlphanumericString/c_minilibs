#pragma once

#include <assert.h>

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "./dsu/rdsu/rewindable_disjointed_set_union.h"

// create
void test_rdsu_new(void);
void test_rdsu_new_from_array(void);
void test_rdsu_init(void);
void test_rdsu_init_from_arrays(void);

// destroy
void test_rdsu_clr(void);
void test_rdsu_destroy(void);
void test_rdsu_free(void);

// getter
void test_rdsu_max_depth(void);
void test_rdsu_size(void);
void test_rdsu_nb_components(void);

// grow
void test_rdsu_grow(void);

// links
void test_rdsu_union(void);
void test_rdsu_find(void);
void test_rdsu_compress_all(void);

// querry
void test_rdsu_is_related(void);
void test_rdsu_validate(void);

// rewind
void test_rdsu_save_state(void);
void test_rdsu_rewind(void);
void test_rdsu_rewind_to_state(void);
void test_rdsu_rewind_clear(void);
