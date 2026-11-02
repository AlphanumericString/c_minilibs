#include <assert.h>

#include "hp_tests.h"

static int cmp_rawptr(const void *a, const void *b) {
	if ((size_t)a < (size_t)b) return (-1);
	else if ((size_t)a > (size_t)b) return (1);
	return (0);
}

static void *dummy_elems[4];

NO_COV void test_hp_is_valid(void) {
	const t_hp ok = {
	    .nb = 2, .capacity = 4, .elems = dummy_elems, .cmp = cmp_rawptr};
	const t_hp no_elems = {
	    .nb = 2, .capacity = 4, .elems = NULL, .cmp = cmp_rawptr};
	const t_hp no_cmp = {
	    .nb = 2, .capacity = 4, .elems = dummy_elems, .cmp = NULL};
	const t_hp nb_inf_cap = {
	    .nb = 5, .capacity = 4, .elems = dummy_elems, .cmp = cmp_rawptr};

	assert(hp_is_valid(&ok) == true);
	assert(hp_is_valid(&no_elems) == false);
	assert(hp_is_valid(&no_cmp) == false);
	assert(hp_is_valid(&nb_inf_cap) == false);
	assert(hp_is_valid(NULL) == false);
}
NO_COV void test_hp_is_empty(void) {
	const t_hp empty = {
	    .nb = 0, .capacity = 4, .elems = dummy_elems, .cmp = cmp_rawptr};
	const t_hp not_empty = {
	    .nb = 2, .capacity = 4, .elems = dummy_elems, .cmp = cmp_rawptr};

	assert(hp_is_empty(&empty) == true);
	assert(hp_is_empty(&not_empty) == false);
	assert(hp_is_empty(NULL) == false);
}
NO_COV void test_hp_is_full(void) {
	const t_hp full = {
	    .nb = 4, .capacity = 4, .elems = dummy_elems, .cmp = cmp_rawptr};
	const t_hp not_full = {
	    .nb = 2, .capacity = 4, .elems = dummy_elems, .cmp = cmp_rawptr};
	const t_hp no_elems = {
	    .nb = 4, .capacity = 4, .elems = NULL, .cmp = cmp_rawptr};

	assert(hp_is_full(&full) == true);
	assert(hp_is_full(&not_full) == false);
	assert(hp_is_full(&no_elems) == false);
	assert(hp_is_full(NULL) == false);
}
