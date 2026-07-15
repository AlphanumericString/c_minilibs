#pragma once

#include <assert.h>

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "dsu/disjointed_set_union.h"

#define DISP_DSU(dsu)                                                          \
	do {                                                                       \
		DBG("DSU INFO\n"                                                       \
		    "\t.size:%zu\n"                                                    \
		    "\t.single_alloc:%s\n"                                             \
		    "\t.parent:%p\n"                                                   \
		    "\t.rank:%p\n"                                                     \
		    "\t.nb_set:%zu\n",                                                 \
		    (dsu).size, (dsu).rp_single_alloc ? "true" : "no", (dsu).parent,   \
		    (dsu).rank, (dsu).components);                                     \
		if (!(dsu).parent || !(dsu).rank) {                                    \
			DBG("\tparents - or - rank missing\n");                            \
			break;                                                             \
		}                                                                      \
		DBG("\t%8s | %-8s\n", "parent", "rank");                               \
		for (size_t i = 0; i < (dsu).size; i++)                                \
			DBG("\t%8d | %-8d\n", (dsu).parent[i], (dsu).rank[i]);             \
	} while (0)

// create
void test_dsu_new(void);
void test_dsu_new_from_array(void);
void test_dsu_init(void);
void test_dsu_init_from_arrays(void);

// grow
void test_dsu_grow(void);

// destroy
void test_dsu_clr(void);
void test_dsu_destroy(void);
void test_dsu_free(void);

// getter
void test_dsu_max_depth(void);
void test_dsu_size(void);
void test_dsu_nb_components(void);

// querry
void test_dsu_is_related(void);
void test_dsu_strict_is_related(void);
void test_dsu_validate(void);

// links
void test_dsu_union(void);
void test_dsu_find(void);
void test_dsu_strict_find(void);
void test_dsu_compress_all(void);
