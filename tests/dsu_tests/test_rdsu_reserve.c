#include <assert.h>

#include "rdsu_tests.h"

static size_t count_reserved(const t_cm_rdsu *rdsu) {
	const t_cm_rdsu_log *entry = rdsu->reserved_logs;
	size_t               nb    = 0;

	while (entry) {
		nb++;
		entry = entry->nxt;
	}
	return (nb);
}

NO_COV void test_cm_rdsu_reserve(void) {
	t_cm_rdsu     *rdsu = rdsu_new(10);
	t_cm_rdsu      intact_copy;
	t_cm_rdsu      malformed;
	t_cm_rdsu_log *head;
	t_cm_rdsu_log *checkpoint;

	assert(rdsu_reserved_logs_nb(rdsu) == 0);
	assert(rdsu->reserved_logs == NULL);

	// reserving nothing is a successful no-op
	alloc_reset_counter();
	assert(rdsu_reserve(rdsu, 0) == true);
	assert(nb_alloc == 0);
	assert(rdsu_reserved_logs_nb(rdsu) == 0);

	// reserve a batch of logs
	alloc_reset_counter();
	assert(rdsu_reserve(rdsu, 4) == true);
	assert(nb_alloc == 4);
	assert(rdsu_reserved_logs_nb(rdsu) == 4);
	assert(count_reserved(rdsu) == 4);

	// already enough logs: no allocation
	alloc_reset_counter();
	assert(rdsu_reserve(rdsu, 2) == true);
	assert(nb_alloc == 0);
	assert(rdsu_reserved_logs_nb(rdsu) == 4);
	assert(count_reserved(rdsu) == 4);

	// only the missing logs are allocated
	alloc_reset_counter();
	assert(rdsu_reserve(rdsu, 7) == true);
	assert(nb_alloc == 3);
	assert(rdsu_reserved_logs_nb(rdsu) == 7);
	assert(count_reserved(rdsu) == 7);

	// a union consumes a reserved log instead of allocating
	alloc_reset_counter();
	assert(rdsu_union(rdsu, 0, 1) == true);
	assert(nb_alloc == 0);
	assert(rdsu->change_log != NULL);
	assert(rdsu_reserved_logs_nb(rdsu) == 6);
	assert(count_reserved(rdsu) == 6);

	// rewinding gives the log back to the reserve
	alloc_reset_counter();
	assert(rdsu_rewind(rdsu) == true);
	assert(nb_alloc == 0);
	assert(rdsu->change_log == NULL);
	assert(rdsu_reserved_logs_nb(rdsu) == 7);
	assert(count_reserved(rdsu) == 7);

	// a checkpoint is still valid after reserving more logs
	assert(rdsu_union(rdsu, 0, 1) == true);
	checkpoint = rdsu_save_state(rdsu);
	assert(checkpoint != NULL);
	assert(rdsu_union(rdsu, 2, 3) == true);
	assert(rdsu_reserve(rdsu, 12) == true);
	assert(rdsu_reserved_logs_nb(rdsu) == 12);
	assert(rdsu_rewind_to_state(rdsu, checkpoint) == true);
	assert(rdsu_reserved_logs_nb(rdsu) == 13);
	assert(count_reserved(rdsu) == 13);

	// failed reservation leaves the reserve untouched
	intact_copy = *rdsu;
	head        = rdsu->reserved_logs;
	alloc_reset_counter();
	alloc_set_call_limit(2);
	assert(rdsu_reserve(rdsu, 20) == false);
	alloc_set_call_limit(ALLOC_DFT_LIMIT);
	assert(rdsu->reserved_logs == head);
	assert(rdsu_reserved_logs_nb(rdsu) == 13);
	assert(count_reserved(rdsu) == 13);
	assert(rdsu->change_log == intact_copy.change_log);
	assert(rdsu->parent == intact_copy.parent);
	assert(rdsu->rank == intact_copy.rank);

	assert(rdsu_reserve(NULL, 1) == false);
	malformed        = *rdsu;
	malformed.parent = NULL;
	assert(rdsu_reserve(&malformed, 1) == false);
	malformed      = *rdsu;
	malformed.rank = NULL;
	assert(rdsu_reserve(&malformed, 1) == false);

	// clear/reuse paths keep the counter in sync
	rdsu_rewind_clear(rdsu);
	assert(rdsu_reserved_logs_nb(rdsu) == 0);
	assert(count_reserved(rdsu) == 0);
	assert(rdsu_reserve(rdsu, 2) == true);
	assert(rdsu_reserved_logs_nb(rdsu) == 2);
	assert(count_reserved(rdsu) == 2);

	rdsu_free(&rdsu);
}
