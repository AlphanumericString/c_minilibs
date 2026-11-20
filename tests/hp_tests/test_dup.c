#include <assert.h>
#include <string.h>

#include "hp_tests.h"

static void build_min(t_cm_hp *h, int *vals, size_t n) {
	for (size_t i = 0; i < n; i++)
		assert(hp_min_push(h, &vals[i]) == true);
}

NO_COV void test_cm_hp_dup(void) {
	int      vals[] = {42, 17, 99, 3, 56};
	t_cm_hp *src;
	t_cm_hp *dup;
	t_cm_hp *empty;
	t_cm_hp *empty_dup;
	int      out;

	src = hp_create(6, sizeof(int), cb_cmp_int);
	assert(src != NULL);
	build_min(src, vals, 5);

	dup = hp_dup(src);
	assert(dup != NULL);
	assert(dup != src);
	assert(dup->nb == src->nb);
	assert(dup->capacity == src->capacity);
	assert(dup->sz_e == src->sz_e);
	assert(dup->cmp == src->cmp);
	assert(dup->elems != src->elems);
	assert(memcmp(dup->elems, src->elems, src->nb * src->sz_e) == 0);
	assert(hp_min_is_valid(dup) == true);

	assert(hp_min_pop(src, &out) == true);
	assert(dup->nb == 5);
	assert(hp_min_pop(dup, &out) == true);
	assert(out == 3);
	assert(hp_min_push(dup, &(int){0}) == true);
	assert(src->nb == 4);
	hp_free(&src);
	hp_free(&dup);

	empty = hp_create(4, sizeof(int), cb_cmp_int);
	assert(empty != NULL);
	empty_dup = hp_dup(empty);
	assert(empty_dup != NULL);
	assert(empty_dup->nb == 0);
	assert(empty_dup->capacity == 4);
	assert(hp_is_valid(empty_dup) == true);
	hp_free(&empty);
	hp_free(&empty_dup);

	assert(hp_dup(NULL) == NULL);
	assert(hp_dup(&(t_cm_hp){0}) == NULL);
	assert(hp_dup(&(t_cm_hp){.nb       = 0,
	                         .capacity = 0,
	                         .sz_e     = sizeof(int),
	                         .elems    = &(int){0},
	                         .cmp      = cb_cmp_int}) == NULL);

	src = hp_create(4, sizeof(int), cb_cmp_int);
	assert(src != NULL);
	alloc_reset_counter();
	alloc_set_call_limit(0);
	assert(hp_dup(src) == NULL);
	alloc_reset_counter();
	alloc_set_call_limit(1);
	assert(hp_dup(src) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	hp_free(&src);
}

NO_COV void test_cm_hp_ndup(void) {
	int      vals[] = {42, 17, 99, 3, 56};
	t_cm_hp *src;
	t_cm_hp *dup;
	t_cm_hp *empty;
	int      out;

	src = hp_create(8, sizeof(int), cb_cmp_int);
	assert(src != NULL);
	build_min(src, vals, 5);

	dup = hp_ndup(src, 3);
	assert(dup != NULL);
	assert(dup->nb == 3);
	assert(dup->capacity == 3);
	assert(dup->sz_e == src->sz_e);
	assert(dup->cmp == src->cmp);
	assert(memcmp(dup->elems, src->elems, 3 * src->sz_e) == 0);
	assert(hp_min_is_valid(dup) == true);
	for (int prev = -1, i = 0; i < 3; i++) {
		assert(hp_min_pop(dup, &out) == true);
		assert(out >= prev);
		prev = out;
	}
	assert(hp_min_pop(dup, &out) == false);
	hp_free(&dup);

	dup = hp_ndup(src, 99);
	assert(dup != NULL);
	assert(dup->nb == src->nb);
	assert(dup->capacity == src->nb);
	assert(memcmp(dup->elems, src->elems, src->nb * src->sz_e) == 0);
	hp_free(&dup);

	assert(hp_ndup(src, 0) == NULL);
	assert(hp_ndup(NULL, 3) == NULL);
	assert(hp_ndup(&(t_cm_hp){0}, 3) == NULL);

	empty = hp_create(4, sizeof(int), cb_cmp_int);
	assert(empty != NULL);
	assert(hp_ndup(empty, 3) == NULL);
	hp_free(&empty);
	hp_free(&src);

	src = hp_create(4, sizeof(int), cb_cmp_int);
	assert(src != NULL);
	assert(hp_min_push(src, &vals[0]) == true);
	alloc_reset_counter();
	alloc_set_call_limit(0);
	assert(hp_ndup(src, 1) == NULL);
	alloc_reset_counter();
	alloc_set_call_limit(1);
	assert(hp_ndup(src, 1) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	hp_free(&src);
}
