#include <assert.h>

#include "da_tests.h"

NO_COV void test_da_is_valid(void) {
	const t_da da_ok1 = {
	    .array = (int *)arr_5, .cap_e = 5, .nb_e = 5, .sz_e = sizeof(int)};
	const t_da cap_inf_nb_e = {
	    .array = (int *)arr_5, .cap_e = 4, .nb_e = 5, .sz_e = sizeof(int)};
	const t_da no_arr = {
	    .array = NULL, .cap_e = 5, .nb_e = 5, .sz_e = sizeof(int)};
	const t_da no_size_e = {
	    .array = (int *)arr_5, .cap_e = 5, .nb_e = 5, .sz_e = 0};

	assert(da_is_valid(&da_ok1) == true);
	assert(da_is_valid(&cap_inf_nb_e) == false);
	assert(da_is_valid(&no_arr) == false);
	assert(da_is_valid(&no_size_e) == false);
	assert(da_is_valid(NULL) == false);
}
NO_COV void test_da_is_not_null(void) {
	const t_da da_ok1 = {
	    .array = (int *)arr_5, .cap_e = 5, .nb_e = 5, .sz_e = sizeof(int)};
	const t_da no_arr = {
	    .array = NULL, .cap_e = 5, .nb_e = 5, .sz_e = sizeof(int)};
	const t_da no_sz = {
	    .array = (int *)arr_5, .cap_e = 5, .nb_e = 5, .sz_e = 0};
	const t_da no_cap = {
	    .array = (int *)arr_5, .cap_e = 0, .nb_e = 5, .sz_e = sizeof(int)};

	assert(da_is_not_null(&da_ok1) == true);
	assert(da_is_not_null(NULL) == false);
	assert(da_is_not_null(&no_arr) == false);
	assert(da_is_not_null(&no_sz) == false);
	assert(da_is_not_null(&no_cap) == false);
}
NO_COV void test_da_is_empty(void) {
	const t_da da_ok1 = {
	    .array = (int *)arr_5, .cap_e = 5, .nb_e = 5, .sz_e = sizeof(int)};
	const t_da da_empty = {
	    .array = (int *)arr_5, .cap_e = 5, .nb_e = 0, .sz_e = sizeof(int)};
	const t_da no_arr = {
	    .array = NULL, .cap_e = 5, .nb_e = 5, .sz_e = sizeof(int)};
	const t_da no_sz = {
	    .array = (int *)arr_5, .cap_e = 5, .nb_e = 5, .sz_e = 0};
	const t_da no_cap = {
	    .array = (int *)arr_5, .cap_e = 0, .nb_e = 5, .sz_e = sizeof(int)};

	assert(da_is_empty(&da_ok1) == false);
	assert(da_is_empty(&da_empty) == true);
	assert(da_is_empty(NULL) == false);
	assert(da_is_empty(&no_arr) == false);
	assert(da_is_empty(&no_sz) == false);
	assert(da_is_empty(&no_cap) == false);
}
NO_COV void test_da_is_full(void) {
	const t_da da_full = {
	    .array = (int *)arr_5, .cap_e = 5, .nb_e = 5, .sz_e = sizeof(int)};
	const t_da da_empty = {
	    .array = (int *)arr_5, .cap_e = 5, .nb_e = 0, .sz_e = sizeof(int)};
	const t_da no_arr = {
	    .array = NULL, .cap_e = 5, .nb_e = 5, .sz_e = sizeof(int)};
	const t_da no_sz = {
	    .array = (int *)arr_5, .cap_e = 5, .nb_e = 5, .sz_e = 0};
	const t_da no_cap = {
	    .array = (int *)arr_5, .cap_e = 0, .nb_e = 5, .sz_e = sizeof(int)};

	assert(da_is_full(&da_full) == true);
	assert(da_is_full(&da_empty) == false);
	assert(da_is_full(NULL) == false);
	assert(da_is_full(&no_arr) == false);
	assert(da_is_full(&no_sz) == false);
	assert(da_is_full(&no_cap) == false);
}
