#pragma once

#ifndef C_MINI__TESTS
#	define C_MINI__TESTS
#endif
#include "common_base/tests.h"
#include "dsu/dsu_convert/dsu_convertions.h"

void test_cdsu_new_from_rdsu(void);
void test_cdsu_init_from_rdsu(void);
void test_cdsu_to_rdsu(void);

void test_rdsu_new_from_cdsu(void);
void test_rdsu_init_from_cdsu(void);
void test_rdsu_to_cdsu(void);
