#include <assert.h>

#include "dsuc_tests.h"

// -- cdsu --
NO_COV void test_cm_cdsu_new_from_rdsu(void) {
	t_cm_rdsu *rdsu;
	t_cm_cdsu *cdsu;

	rdsu = rdsu_new(5);
	assert(rdsu != NULL);
	assert(rdsu_union(rdsu, 0, 1) == true);
	assert(rdsu_union(rdsu, 2, 3) == true);
	assert(rdsu_union(rdsu, 3, 4) == true);

	cdsu = cdsu_new_from_rdsu(rdsu);
	assert(cdsu != NULL);
	assert(cdsu->size == rdsu->size);
	assert(cdsu->components == rdsu->components);
	assert(cdsu->rp_single_alloc == true);
	for (size_t i = 0; i < cdsu->size; i++) {
		assert(cdsu->parent[i] == rdsu->parent[i]);
		assert(cdsu->rank[i] == rdsu->rank[i]);
	}
	assert(cdsu_validate(cdsu));
	cdsu_free(&cdsu);

	assert(cdsu_new_from_rdsu(NULL) == NULL);
	alloc_set_call_limit(0);
	assert(cdsu_new_from_rdsu(rdsu) == NULL);
	alloc_reset_counter();
	alloc_set_call_limit(1);
	assert(cdsu_new_from_rdsu(rdsu) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	rdsu_free(&rdsu);
}

NO_COV void test_cm_cdsu_init_from_rdsu(void) {
	t_cm_rdsu *rdsu;
	t_cm_cdsu *cdsu;
	t_cm_cdsu *small;

	rdsu = rdsu_new(5);
	assert(rdsu != NULL);
	assert(rdsu_union(rdsu, 0, 1) == true);
	assert(rdsu_union(rdsu, 2, 3) == true);

	cdsu = cdsu_new(8);
	assert(cdsu != NULL);
	assert(cdsu_init_from_rdsu(cdsu, rdsu) == cdsu);
	assert(cdsu->size == rdsu->size);
	assert(cdsu->components == rdsu->components);
	for (size_t i = 0; i < cdsu->size; i++) {
		assert(cdsu->parent[i] == rdsu->parent[i]);
		assert(cdsu->rank[i] == rdsu->rank[i]);
	}
	assert(cdsu_validate(cdsu));

	assert(cdsu_init_from_rdsu(NULL, rdsu) == NULL);
	assert(cdsu_init_from_rdsu(cdsu, NULL) == NULL);

	small = cdsu_new(3);
	assert(small != NULL);
	assert(cdsu_init_from_rdsu(small, rdsu) == NULL);
	cdsu_free(&small);

	cdsu_free(&cdsu);
	rdsu_free(&rdsu);
}
NO_COV void test_cm_cdsu_to_rdsu(void) {
	t_cm_cdsu *cdsu = cdsu_new(5);
	t_cm_rdsu *rdsu;

	assert(cdsu != NULL);
	assert(cdsu_union(cdsu, 0, 1) == true);
	assert(cdsu_union(cdsu, 2, 3) == true);

	rdsu = cdsu_to_rdsu(&cdsu);
	assert(rdsu != NULL);
	assert(rdsu_find(rdsu, 0) == rdsu_find(rdsu, 1));
	assert(rdsu_find(rdsu, 2) == rdsu_find(rdsu, 3));
	assert(cdsu == NULL);
	rdsu_free(&rdsu);

	cdsu = NULL;
	assert(cdsu_to_rdsu(&cdsu) == NULL);
	assert(cdsu_to_rdsu(NULL) == NULL);
	cdsu = cdsu_new(5);
	alloc_set_call_limit(0);
	assert(cdsu_to_rdsu(&cdsu) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(cdsu != NULL);
	cdsu_free(&cdsu);
}

// -- rdsu --
NO_COV void test_cm_rdsu_new_from_cdsu(void) {
	t_cm_cdsu *cdsu;
	t_cm_rdsu *rdsu;

	cdsu = cdsu_new(5);
	assert(cdsu != NULL);
	assert(cdsu_union(cdsu, 0, 1) == true);
	assert(cdsu_union(cdsu, 2, 3) == true);
	assert(cdsu_union(cdsu, 3, 4) == true);

	rdsu = rdsu_new_from_cdsu(cdsu);
	assert(rdsu != NULL);
	assert(rdsu->size == cdsu->size);
	assert(rdsu->components == cdsu->components);
	assert(rdsu->rp_single_alloc == true);
	assert(rdsu->change_log == NULL);
	for (size_t i = 0; i < rdsu->size; i++) {
		assert(rdsu->parent[i] == cdsu->parent[i]);
		assert(rdsu->rank[i] == cdsu->rank[i]);
	}
	assert(rdsu_validate(rdsu));
	rdsu_free(&rdsu);

	assert(rdsu_new_from_cdsu(NULL) == NULL);
	alloc_set_call_limit(0);
	assert(rdsu_new_from_cdsu(cdsu) == NULL);
	alloc_reset_counter();
	alloc_set_call_limit(1);
	assert(rdsu_new_from_cdsu(cdsu) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);

	cdsu_free(&cdsu);
}

NO_COV void test_cm_rdsu_init_from_cdsu(void) {
	t_cm_cdsu *cdsu;
	t_cm_rdsu *rdsu;
	t_cm_rdsu *small;

	cdsu = cdsu_new(5);
	assert(cdsu != NULL);
	assert(cdsu_union(cdsu, 0, 1) == true);
	assert(cdsu_union(cdsu, 2, 3) == true);

	rdsu = rdsu_new(8);
	assert(rdsu != NULL);
	assert(rdsu_union(rdsu, 0, 1) == true);
	assert(rdsu->change_log != NULL);
	assert(rdsu->reserved_logs == NULL);
	assert(rdsu_init_from_cdsu(rdsu, cdsu) == rdsu);
	assert(rdsu->size == cdsu->size);
	assert(rdsu->components == cdsu->components);
	assert(rdsu->change_log == NULL);
	for (size_t i = 0; i < rdsu->size; i++) {
		assert(rdsu->parent[i] == cdsu->parent[i]);
		assert(rdsu->rank[i] == cdsu->rank[i]);
	}
	assert(rdsu_validate(rdsu));

	// with reserved logs
	assert(rdsu_union(rdsu, 4, 3) == true);
	assert(rdsu_rewind(rdsu) == true);
	assert(rdsu->change_log == NULL);
	assert(rdsu->reserved_logs != NULL);
	assert(rdsu_init_from_cdsu(rdsu, cdsu) == rdsu);
	assert(rdsu->size == cdsu->size);
	assert(rdsu->components == cdsu->components);
	assert(rdsu->change_log == NULL);
	for (size_t i = 0; i < rdsu->size; i++) {
		assert(rdsu->parent[i] == cdsu->parent[i]);
		assert(rdsu->rank[i] == cdsu->rank[i]);
	}
	assert(rdsu_validate(rdsu));

	assert(rdsu_init_from_cdsu(NULL, cdsu) == NULL);
	assert(rdsu_init_from_cdsu(rdsu, NULL) == NULL);

	small = rdsu_new(3);
	assert(small != NULL);
	assert(rdsu_init_from_cdsu(small, cdsu) == NULL);
	rdsu_free(&small);

	rdsu_free(&rdsu);
	cdsu_free(&cdsu);
}
void test_cm_rdsu_to_cdsu(void) {
	t_cm_rdsu *rdsu = rdsu_new(5);
	t_cm_cdsu *cdsu;

	assert(rdsu != NULL);
	assert(rdsu_union(rdsu, 0, 1) == true);
	assert(rdsu_union(rdsu, 2, 3) == true);

	cdsu = rdsu_to_cdsu(&rdsu);
	assert(cdsu != NULL);
	assert(cdsu_find(cdsu, 0) == cdsu_find(cdsu, 1));
	assert(cdsu_find(cdsu, 2) == cdsu_find(cdsu, 3));
	assert(rdsu == NULL);
	cdsu_free(&cdsu);

	rdsu = NULL;
	assert(rdsu_to_cdsu(&rdsu) == NULL);
	assert(rdsu_to_cdsu(NULL) == NULL);
	rdsu = rdsu_new(5);
	alloc_set_call_limit(0);
	assert(rdsu_to_cdsu(&rdsu) == NULL);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(rdsu != NULL);
	rdsu_free(&rdsu);
}
